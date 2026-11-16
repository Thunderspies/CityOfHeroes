#############################################################
## ChildFX_Droplet_Falling.fx
#############################################################

FxInfo
Lifespan 60

#############################################################


Condition
	On	Time
	Time	0



	Event
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 40 0
		End
		Part	:Offset_WaterDroplet.part
		Lifespan 60

	End
End

Condition
	On	Time
	Time	45

	Event
		Type	Local
		At	Origin
		Part	:Offset_Splash.part
		SoundNoRepeat 3
		Sound CaveDrip1_01 50 50 .1
		Sound CaveDrip1_02 50 50 .05
		Sound CaveDrip1_03 50 50 .1
		Sound CaveDrip1_04 50 50 .05
		Sound CaveDrip1_05 50 50 .1
		Sound CaveDrip1_06 50 50 .05
		Sound CaveDrip1_07 50 50 .1
		Lifespan 	1
	End
End