import subprocess
import re
import csv
import sys
import os

# ==============================================================================
# CONFIGURAÇÕES E PARÂMETROS DO TP02
# ==============================================================================
METODO = 3  # 3: Quicksort Externo

# Quantidades estritamente especificadas no tp02.pdf (em ordem crescente)
QUANTIDADES = [100, 200, 2000, 20000, 200000, 471705]

# Situações de ordem inicial do arquivo
SITUACOES = {
    1: "Crescente",
    2: "Decrescente",
    3: "Aleatorio"
}

TIMEOUT_SEGUNDOS = 120  # Tolerância máxima de 2 minutos por execução
ARQUIVO_CSV = "resultados_quicksort.csv"
EXECUTAVEL = "./ordena"

def restaurar_arquivos_originais():
    """Restaura os arquivos binários originais chamando 'make original'."""
    try:
        subprocess.run(
            ["make", "original"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=True
        )
    except subprocess.CalledProcessError as e:
        print(f"\n[ERRO CRÍTICO] Falha ao executar 'make original':\n{e.stderr}")
        sys.exit(1)

def extrair_metricas(saida_texto):
    """
    Extrai as 4 métricas obrigatórias da saída do executável via Regex:
    - Tempo de execução
    - Número de comparações
    - Transferências de leitura
    - Transferências de escrita
    """
    tempo_match = re.search(r"Tempo de execucao:\s*([\d\.]+)", saida_texto)
    comp_match = re.search(r"Numero de comparacoes:\s*(\d+)", saida_texto)
    leit_match = re.search(r"Numero de transferencias \\(Leitura\\):\s*(\d+)", saida_texto)
    
    # Suporta tanto o print padrão quanto eventuais typos na main original
    escr_match = re.search(r"Numero de (?:transferencias|comparacoes) \\(Escritas\\):\s*(\d+)", saida_texto)

    if not (tempo_match and comp_match and leit_match and escr_match):
        return None

    return {
        "tempo_execucao": float(tempo_match.group(1)),
        "comparacoes": int(comp_match.group(1)),
        "transferencias_leitura": int(leit_match.group(1)),
        "transferencias_escrita": int(escr_match.group(1))
    }

def main():
    if not os.path.exists(EXECUTAVEL):
        print(f"[ERRO] Executável '{EXECUTAVEL}' não foi encontrado no diretório atual.")
        print("Compile o projeto com o 'make' antes de executar este script.")
        sys.exit(1)

    print("=" * 75)
    print(" INICIANDO EXPERIMENTOS DO QUICKSORT EXTERNO (TP02 - ED2)")
    print(f" Tolerância Máxima: {TIMEOUT_SEGUNDOS}s (2 minutos) por teste")
    print(f" Arquivo de saída:  {ARQUIVO_CSV}")
    print("=" * 75)

    with open(ARQUIVO_CSV, mode="w", newline="", encoding="utf-8") as f_csv:
        campos = [
            "metodo",
            "cod_situacao",
            "situacao",
            "quantidade",
            "tempo_execucao",
            "comparacoes",
            "transferencias_leitura",
            "transferencias_escrita"
        ]
        writer = csv.DictWriter(f_csv, fieldnames=campos)
        writer.writeheader()

        for cod_sit, nome_sit in SITUACOES.items():
            print(f"\n>>> INICIANDO SITUAÇÃO {cod_sit}: {nome_sit.upper()} <<<")
            estourou_tempo = False  # Flag resetada a cada nova situação

            for qtd in QUANTIDADES:
                # --------------------------------------------------------------
                # CASO 1: A situação já estourou o tempo em uma quantidade menor
                # --------------------------------------------------------------
                if estourou_tempo:
                    print(f"  [-] Quantidade {qtd:>7,}: PULADO com '-' (excedeu 2 min anteriormente)")
                    writer.writerow({
                        "metodo": "Quicksort Externo",
                        "cod_situacao": cod_sit,
                        "situacao": nome_sit,
                        "quantidade": qtd,
                        "tempo_execucao": "-",
                        "comparacoes": "-",
                        "transferencias_leitura": "-",
                        "transferencias_escrita": "-"
                    })
                    f_csv.flush()
                    continue

                # --------------------------------------------------------------
                # CASO 2: Execução normal com controle de timeout
                # --------------------------------------------------------------
                print(f"  [*] Executando para {qtd:>7,} registros...", end="", flush=True)

                # Garante arquivos originais no disco antes da ordenação in situ
                restaurar_arquivos_originais()

                comando = [EXECUTAVEL, str(METODO), str(qtd), str(cod_sit)]

                try:
                    processo = subprocess.run(
                        comando,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE,
                        text=True,
                        timeout=TIMEOUT_SEGUNDOS,
                        check=True
                    )
                    
                    metricas = extrair_metricas(processo.stdout)

                    if metricas:
                        writer.writerow({
                            "metodo": "Quicksort Externo",
                            "cod_situacao": cod_sit,
                            "situacao": nome_sit,
                            "quantidade": qtd,
                            "tempo_execucao": f"{metricas['tempo_execucao']:.6f}",
                            "comparacoes": metricas["comparacoes"],
                            "transferencias_leitura": metricas["transferencias_leitura"],
                            "transferencias_escrita": metricas["transferencias_escrita"]
                        })
                        f_csv.flush()

                        print(f" OK! ({metricas['tempo_execucao']:.4f}s | "
                              f"Comp: {metricas['comparacoes']:,} | "
                              f"L: {metricas['transferencias_leitura']:,} | "
                              f"E: {metricas['transferencias_escrita']:,})")
                    else:
                        print(" [AVISO] Falha ao extrair métricas do terminal. Registrado como '-'.")
                        writer.writerow({
                            "metodo": "Quicksort Externo",
                            "cod_situacao": cod_sit,
                            "situacao": nome_sit,
                            "quantidade": qtd,
                            "tempo_execucao": "-",
                            "comparacoes": "-",
                            "transferencias_leitura": "-",
                            "transferencias_escrita": "-"
                        })
                        f_csv.flush()

                # --------------------------------------------------------------
                # CASO 3: Estouro do tempo limite de 2 minutos
                # --------------------------------------------------------------
                except subprocess.TimeoutExpired:
                    print(f" TIMEOUT (> {TIMEOUT_SEGUNDOS}s)! Registrando '-' e pulando maiores.")
                    estourou_tempo = True
                    writer.writerow({
                        "metodo": "Quicksort Externo",
                        "cod_situacao": cod_sit,
                        "situacao": nome_sit,
                        "quantidade": qtd,
                        "tempo_execucao": "-",
                        "comparacoes": "-",
                        "transferencias_leitura": "-",
                        "transferencias_escrita": "-"
                    })
                    f_csv.flush()
                    # Limpa o arquivo que foi interrompido no meio da escrita
                    restaurar_arquivos_originais()

                # --------------------------------------------------------------
                # CASO 4: Erro de execução (Crash / Falha de Segmentação)
                # --------------------------------------------------------------
                except subprocess.CalledProcessError as e:
                    print(f" ERRO NA EXECUÇÃO (Código {e.returncode})! Registrado como '-'.")
                    writer.writerow({
                        "metodo": "Quicksort Externo",
                        "cod_situacao": cod_sit,
                        "situacao": nome_sit,
                        "quantidade": qtd,
                        "tempo_execucao": "-",
                        "comparacoes": "-",
                        "transferencias_leitura": "-",
                        "transferencias_escrita": "-"
                    })
                    f_csv.flush()

    # Restauração final ao encerrar todos os testes
    restaurar_arquivos_originais()
    print("\n" + "=" * 75)
    print(f" Bateria de testes finalizada! Resultados salvos em: {ARQUIVO_CSV}")
    print("=" * 75)

if __name__ == "__main__":
    main()
