package tech.sovereign.player;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.graphics.Color;
import android.view.Gravity;

public class MainActivity extends Activity implements SurfaceHolder.Callback {
    private SovereignPlayer player;
    private SurfaceView surfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        player = new SovereignPlayer();
        player.initialize();

        FrameLayout layout = new FrameLayout(this);
        layout.setBackgroundColor(Color.BLACK);

        surfaceView = new SurfaceView(this);
        surfaceView.getHolder().addCallback(this);
        layout.addView(surfaceView);

        TextView hud = new TextView(this);
        hud.setText("▶ Sovereign Media Player — Baseline Open-Core\nStatus: Active (Zero-Copy HW Engine)");
        hud.setTextColor(Color.WHITE);
        hud.setPadding(40, 40, 40, 40);
        hud.setGravity(Gravity.TOP | Gravity.START);
        layout.addView(hud);

        setContentView(layout);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        player.openMedia("default_stream.mp4");
        player.setSurface(holder.getSurface());
        player.play();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {}

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        player.pause();
        player.release();
    }
}
