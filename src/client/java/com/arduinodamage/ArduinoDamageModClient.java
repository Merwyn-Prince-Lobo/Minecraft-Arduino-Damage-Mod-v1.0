package com.arduinodamage;

import net.fabricmc.api.ClientModInitializer;
import net.fabricmc.fabric.api.client.event.lifecycle.v1.ClientTickEvents;
import net.minecraft.client.Minecraft;
import net.minecraft.world.entity.player.Player;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.PrintWriter;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ArduinoDamageModClient implements ClientModInitializer {

    private static final Logger LOGGER = LoggerFactory.getLogger(ArduinoDamageMod.MOD_ID);
    private static final int BRIDGE_PORT = 25575;
    private float lastHealth = -1f;
    private final ExecutorService executor = Executors.newSingleThreadExecutor();
    private long lastTriggerTime = 0;
    private static final long COOLDOWN_MS = 400;

    @Override
    public void onInitializeClient() {
        LOGGER.info("ArduinoDamageMod client ready — monitoring player health.");

        ClientTickEvents.END_CLIENT_TICK.register(client -> {
            Minecraft mc = Minecraft.getInstance();
            Player player = mc.player;

            if (player == null) { lastHealth = -1f; return; }

            float currentHealth = player.getHealth();
            if (lastHealth < 0) { lastHealth = currentHealth; return; }

            if (currentHealth < lastHealth) {
                long now = System.currentTimeMillis();
                if (now - lastTriggerTime >= COOLDOWN_MS) {
                    lastTriggerTime = now;
                    LOGGER.info("Damage taken: {} HP — triggering Arduino", lastHealth - currentHealth);
                    sendDamageSignal();
                }
            }
            lastHealth = currentHealth;
        });
    }

    private void sendDamageSignal() {
        executor.submit(() -> {
            try (Socket socket = new Socket("127.0.0.1", BRIDGE_PORT);
                 PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {
                out.println("DAMAGE");
            } catch (Exception e) {
                LOGGER.debug("Arduino bridge not connected: {}", e.getMessage());
            }
        });
    }
}
