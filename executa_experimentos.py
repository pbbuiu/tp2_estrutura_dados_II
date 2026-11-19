import csv
import re
import subprocess
import sys

QUANTIDADES = [100, 200, 2000, 20000, 200000, 471705]
SITUACOES = {
    1: "Crescente",
    2: "Decrescente",
    3: "Aleatorio",
}
TIMEOUT_SEGUNDOS = 120

REGEX_TEMPO = re.compile(r"-\s*Tempo de execucao:\s*([0-9]+(?:\.[0-9]+)?)")
REGEX_COMP = re.compile(r"-\s*Numero de comparacoes:\s*(\d+)")
REGEX_LEITURA = re.compile(r"-\s*Numero de transferencias \(Leitura\):\s*(\d+)")
REGEX_ESCRITA = re.compile(r"-\s*Numero de transferencias \(Escritas\):\s*(\d+)")


def extrair_metricas(saida: str):
    tempo = REGEX_TEMPO.search(saida)
    comp = REGEX_COMP.search(saida)
    leitura = REGEX_LEITURA.search(saida)
    escrita = REGEX_ESCRITA.search(saida)

    if not all([tempo, comp, leitura, escrita]):
        return None

    return (
        tempo.group(1),
        comp.group(1),
        leitura.group(1),
        escrita.group(1),
    )


def executar_experimentos(metodo: int):
    arquivo_csv = f"resultados_metodo_{metodo}.csv"
    cabecalho = [
        "metodo",
        "cod_situacao",
        "situacao",
        "quantidade",
        "tempo_execucao",
        "comparacoes",
        "transferencias_leitura",
        "transferencias_escrita",
    ]

    with open(arquivo_csv, mode="w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(cabecalho)
        f.flush()

        for cod_situacao, nome_situacao in SITUACOES.items():
            pular_restantes = False

            for qtd in QUANTIDADES:
                if pular_restantes:
                    print(f"[PULADO] Metodo={metodo} | Situacao={nome_situacao} | Qtd={qtd}")
                    writer.writerow([metodo, cod_situacao, nome_situacao, qtd, "-", "-", "-", "-"])
                    f.flush()
                    continue

                print(f"[RODANDO] Metodo={metodo} | Situacao={nome_situacao} | Qtd={qtd}...")

                try:
                    subprocess.run(
                        ["make", "original"],
                        check=True,
                        stdout=subprocess.DEVNULL,
                        stderr=subprocess.PIPE,
                        text=True,
                    )
                except subprocess.CalledProcessError as e:
                    print(f"[ERRO] Falha ao executar 'make original':\n{e.stderr}")
                    sys.exit(1)

                try:
                    proc = subprocess.run(
                        ["./ordena", str(metodo), str(qtd), str(cod_situacao)],
                        capture_output=True,
                        text=True,
                        timeout=TIMEOUT_SEGUNDOS,
                    )
                except subprocess.TimeoutExpired:
                    print(f"[TIMEOUT > {TIMEOUT_SEGUNDOS}s] Metodo={metodo} | Situacao={nome_situacao} | Qtd={qtd}")
                    writer.writerow([metodo, cod_situacao, nome_situacao, qtd, "-", "-", "-", "-"])
                    f.flush()
                    pular_restantes = True
                    continue

                metricas = extrair_metricas(proc.stdout)

                if metricas is None:
                    print(f"[ERRO PARSING] Falha ao extrair metricas (return code: {proc.returncode}).")
                    print("--- STDOUT ---")
                    print(proc.stdout)
                    print("--- STDERR ---")
                    print(proc.stderr)
                    print("--------------")
                    writer.writerow([metodo, cod_situacao, nome_situacao, qtd, "-", "-", "-", "-"])
                else:
                    tempo, comp, leitura, escrita = metricas
                    writer.writerow([metodo, cod_situacao, nome_situacao, qtd, tempo, comp, leitura, escrita])

                f.flush()

    print(f"Concluido. Arquivo salvo em: {arquivo_csv}")


if __name__ == "__main__":
    if len(sys.argv) > 1:
        metodo_escolhido = int(sys.argv[1])
    else:
        metodo_escolhido = int(input("Escolha o metodo (1 ou 3): ").strip())

    if metodo_escolhido not in (1, 3):
        print("Metodo invalido. Use 1 ou 3.")
        sys.exit(1)

    executar_experimentos(metodo_escolhido)