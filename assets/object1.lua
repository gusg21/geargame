local obj1 = {}

obj1.initialize = function(self, scene)
    self.scene = scene
    self.window = Scene_GetWindow(scene)
end

obj1.draw = function(self)
    Window_DrawRectangle(self.window, -8, -8, 16, 16)
end

return obj1