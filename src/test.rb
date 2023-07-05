require 'benchmark'

def brute_force(texte, motif)
    test, comp, count, indmotif = 0, 0, 0, 0
    (texte.length-motif.length).times do |i|
        test=1
        motif.length.times do |j|
            comp+=1
            if texte[i+indmotif] != motif[indmotif]
                test=0
                break
            end
            indmotif+=1
        end
        if test == 1
            # printf("Occurrence en %d avec %d comparaisons\n", i, comp)
            count+=1
        end
        indmotif=0
    end
    printf("nboccu %d\n", count);
end

time = Benchmark.realtime do
    texte = File.read("sequence.fasta").chomp
    motif = "ATAATATA"
    brute_force(texte, motif)
end

puts "Temps: #{(time * 1000).round(2)} ms"