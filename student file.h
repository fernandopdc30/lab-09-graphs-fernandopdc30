import java.util.*;

public class ReconstruirCarreteras {

    public static String reconstruye(String[] carreteras) {
        Map<String, Integer> costoTotal = new HashMap<>();
        Map<String, String> padre = new HashMap<>();

        for (String carretera : carreteras) {
            String[] partes = carretera.split(" ");
            String id = partes[0];
            String ciudad1 = partes[1];
            String ciudad2 = partes[2];

            if (!costoTotal.containsKey(ciudad1)) {
                costoTotal.put(ciudad1, 0);
                padre.put(ciudad1, ciudad1);
            }
            if (!costoTotal.containsKey(ciudad2)) {
                costoTotal.put(ciudad2, 0);
                padre.put(ciudad2, ciudad2);
            }

            int costo = partes.length == 4 ? Integer.parseInt(partes[3]) : 0;
            costoTotal.put(ciudad1, costoTotal.get(ciudad1) + costo);
            costoTotal.put(ciudad2, costoTotal.get(ciudad2) + costo);

            String padreCiudad1 = encontrarPadre(padre, ciudad1);
            String padreCiudad2 = encontrarPadre(padre, ciudad2);

            if (!padreCiudad1.equals(padreCiudad2)) {
                padre.put(padreCiudad1, padreCiudad2);
            }
        }

        Set<String> componentesConexas = new HashSet<>();
        for (String ciudad : costoTotal.keySet()) {
            componentesConexas.add(encontrarPadre(padre, ciudad));
        }

        if (componentesConexas.size() > 1) {
            return "IMPOSIBLE";
        }

        String resultado = "";
        PriorityQueue<String> opciones = new PriorityQueue<>();
        for (String id : costoTotal.keySet()) {
            if (costoTotal.get(id) > 0) {
                opciones.add(id);
            }
        }

        while (!opciones.isEmpty()) {
            resultado += opciones.poll() + " ";
        }

        return resultado.trim();
    }

    private static String encontrarPadre(Map<String, String> padre, String ciudad) {
        if (!padre.get(ciudad).equals(ciudad)) {
            padre.put(ciudad, encontrarPadre(padre, padre.get(ciudad)));
        }
        return padre.get(ciudad);
    }
}
