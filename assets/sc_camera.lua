local cam = {}

function cam:initialize(me, state, window, assets)
    -- print("me", me, "state", state, "window", window, "assets", assets)

    self.me = me
    self.state = state
    self.window = window
    self.assets = assets
end

function cam:ready()
    -- print("ready")

    self.x = 0
    self.tex = Assets_Get(self.assets, ASSET_TEXTURE, "sunset")
    self.diamond_spec = Assets_Get(self.assets, ASSET_ACTOR_SPEC, "as_diamond")

    self.scene = State_GetScene(self.state)
    assert(not IsNull(self.scene), "state doesn't have a scene?")

    Scene_

    self.camera = Scene_GetCamera(self.scene)
    assert(not IsNull(self.camera), "scene doesn't have a camera?")
    Camera_SetZoom(self.camera, 3)
    focus = Scene_GetActorByName(self.scene, "CameraFocus")
    fx, fy = Actor_GetPosition(focus)
    -- print("focus pos", fx, fy)
    Camera_SetPosition(self.camera, fx, fy)
end

function cam:update(dt)
    self.x = self.x + (dt * 10.0)
end

function cam:draw()
    Window_DrawTexture(self.window, self.tex, self.x, 0)
end

return cam