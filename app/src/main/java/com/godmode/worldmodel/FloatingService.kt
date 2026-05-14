package com.godmode.worldmodel

import android.app.Service
import android.graphics.PixelFormat
import android.view.Gravity
import android.view.WindowManager
import android.widget.Button
import android.content.Intent
import android.os.Handler
import android.os.Looper

class FloatingService : Service() {
    private lateinit var wm: WindowManager
    private lateinit var btn: Button
    private var active = false
    private val handler = Handler(Looper.getMainLooper())

    override fun onBind(p0: Intent?) = null

    override fun onCreate() {
        super.onCreate()
        NativeLib.init(cacheDir.absolutePath)
        wm = getSystemService(WINDOW_SERVICE) as WindowManager
        btn = Button(this).apply {
            text = "AI OFF"
            setOnClickListener {
                active = !active
                text = if(active) "AI ON" else "AI OFF"
                if(active) startLoop()
            }
        }

        val params = WindowManager.LayoutParams(
            200, 200, 
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT
        )
        params.gravity = Gravity.TOP or Gravity.RIGHT
        wm.addView(btn, params)
    }

    private fun startLoop() {
        if(!active) return
        val coords = NativeLib.step() // Вызов C++ ИИ
        // Здесь должен быть код клика (через AccessibilityService)
        println("AI Решил нажать на: ${coords[0]}, ${coords[1]}")
        
        handler.postDelayed({ startLoop() }, 500) // Цикл каждые 0.5 сек
    }
}

object NativeLib {
    init { System.loadLibrary("worldmodel") }
    external fun init(path: String)
    external fun step(): IntArray
}