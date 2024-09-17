import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class SQLiteConnection {
    public static void connect() {
        // Database URL: This will create the database file in the current project directory
        String url = "jdbc:sqlite:test.db";
        
        // Connect to SQLite and create the database
        try (Connection conn = DriverManager.getConnection(url)) {
            if (conn != null) {
                System.out.println("A new database has been created.");
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public static void main(String[] args) {
        connect();  // Call the connect method to create the database
    }
}










import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

public class SQLiteCreateTable {
    public static void createNewTable() {
        // SQLite connection string
        String url = "jdbc:sqlite:test.db";
        
        // SQL statement for creating a new table
        String sql = "CREATE TABLE IF NOT EXISTS people (\n"
                   + " id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                   + " name TEXT NOT NULL\n"
                   + ");";
        
        try (Connection conn = DriverManager.getConnection(url);
             Statement stmt = conn.createStatement()) {
            // Create a new table
            stmt.execute(sql);
            System.out.println("Table created or already exists.");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public static void main(String[] args) {
        createNewTable();  // Create table
    }
}









import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.Scanner;

public class SQLiteInsert {
    public static void insert(String name) {
        String url = "jdbc:sqlite:test.db";
        String sql = "INSERT INTO people(name) VALUES(?)";

        try (Connection conn = DriverManager.getConnection(url);
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            // Set the value for the name column
            pstmt.setString(1, name);
            pstmt.executeUpdate();
            System.out.println("Name inserted into the database.");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter name: ");
        String name = scanner.nextLine();
        insert(name);  // Insert the entered name into the database
    }
}











import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class SQLiteSelectTest {

    public static void main(String[] args) {
        // SQLite connection string
        String url = "jdbc:sqlite:test.db";  // Database file
        
        // SQL query to retrieve all data from the table
        String sql = "SELECT * FROM my_table";  // Replace with your table name
        
        try (Connection conn = DriverManager.getConnection(url);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {

            // Loop through the result set and display the data
            while (rs.next()) {
                // Assuming the table has two columns: id (integer) and name (text)
                int id = rs.getInt("id");
                String name = rs.getString("name");
                
                // Print each row of data
                System.out.println("ID: " + id + " | Name: " + name);
            }
        } catch (SQLException e) {
            System.out.println("SQL error: " + e.getMessage());
        }
    }
}








import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@SpringBootApplication
public class MyApplication {

    public static void main(String[] args) {
        SpringApplication.run(MyApplication.class, args);
    }
}

@RestController
class MyController {
    @GetMapping("/")
    public String hello() {
        return "Hello, Spring Boot without Maven!";
    }
}