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