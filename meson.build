project('Práctica 2 de PAV - detección de actividad vocal', 'c')

exe = 'vad'
src = ['src/main_vad.c', 'src/vad.c', 'src/pav_analysis.c', 'src/fic_wave.c']
lib = ['-lm', '-lsndfile']

executable (exe, sources: src, link_args: lib)