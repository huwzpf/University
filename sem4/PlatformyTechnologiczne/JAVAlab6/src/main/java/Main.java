import org.apache.commons.lang3.tuple.MutablePair;
import org.apache.commons.lang3.tuple.Pair;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ForkJoinPool;
import java.util.stream.Stream;


public class Main {

    public static BufferedImage transform(BufferedImage original){
            BufferedImage image = new BufferedImage(original.getWidth(), original.getHeight(), original.getType());
            int red, green, blue, in_rgb, out_rgb;
            Color in_color, out_color;
            for (int i = 0; i < original.getWidth(); i++) {
                for (int j = 0; j < original.getHeight(); j++) {
                    in_rgb = original.getRGB(i, j);
                    in_color = new Color(in_rgb);
                    red = in_color.getRed();
                    blue = in_color.getBlue();
                    green = in_color.getGreen();
                    out_color = new Color(red, blue, green);
                    out_rgb = out_color.getRGB();
                    image.setRGB(i, j, out_rgb);
                }
            }
        return image;
    }

    public static void main(String[] args) {
        long time, avg_time, diff;
        int reps = 5, max_threads = 4;
        for(int i = 1; i <= max_threads; i++){
            avg_time = 0;
            for(int j = 0; j < reps; j ++){
                time = System.currentTimeMillis();
                transform_images(i);
                diff = System.currentTimeMillis() - time;
                avg_time += diff;
            }
            avg_time /= reps;
            System.out.println("For thread pool of : " + i + " average execution time is: " + avg_time);
        }

    }

    private static void transform_images(int n) {
        Path source = Path.of("input/");
        ForkJoinPool pool = new ForkJoinPool(n);

        try {
            pool.submit(() -> {
                try (Stream<Path> stream = Files.list(source)){
                    List<Path> files = stream.toList();
                    Stream<MutablePair<String, BufferedImage>> pairStream = files.stream().parallel().map(v -> {
                        try{
                            return new MutablePair<>(v.toString(), transform(ImageIO.read(v.toFile())));
                        }
                        catch (IOException ex){
                            return new MutablePair<>();
                        }
                    });
                    pairStream.forEach(p ->{
                        try{
                            ImageIO.write(p.getRight(), "jpg", new File(p.getLeft().replace("input", "output")));
                        }
                        catch (IOException ex){
                            System.out.println("Unhandled exception");
                        }
                    });

                }
                catch (IOException ex) {
                    System.out.println("Unhandled exception");
                }

            }).get();
        } catch (InterruptedException | ExecutionException e) {
            System.out.println("Unhandled exception");
        }
    }
}
