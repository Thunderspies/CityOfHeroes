#############################################################
## Mot_Presence.fx
#############################################################

FxInfo
	Flags DontSuppress

###########################################################

Condition
	On Time
	Time 0

	Event
		At Root
		Type StartPositOnly
		CameraSpace
		Bhvr Behaviors\GenericParticleFade.bhvr
		Part :Dust_Fullscreen_02.part
		Part :Dust_Fullscreen_01.part
		Part :Dust_Particles.part
	End
End

Condition
	On Cycle
	Time 250
	Chance 0.500000

	Event
		At Root
		Type Start
		CameraSpace
		Part WORLD\City\DarkAstoria\MotFace.part
		LifeSpan 100
	End
End

Condition
	On Cycle
	Time 100
	Random 1

	Event
		At Root
		Type Start
		CameraSpace
		Part WORLD\City\DarkAstoria\ScaryFlashes.part
		LifeSpan 100
	End

	Event
		At Root
		Type Start
		CameraSpace
		Part WORLD\City\DarkAstoria\ScaryFlashes02.part
		LifeSpan 100
	End

	Event
		At Root
		Type Start
		CameraSpace
		Part WORLD\City\DarkAstoria\ScaryFlashes03.part
		LifeSpan 100
	End
End

Condition
	On Cycle
	Time 80
	Random 1

	Event
		At Root
		Type Start
		CameraSpace
		Part WORLD\City\DarkAstoria\ScaryFlashes.part
		LifeSpan 100
	End

	Event
		At Root
		Type Start
		CameraSpace
		Part WORLD\City\DarkAstoria\ScaryFlashes02.part
		LifeSpan 100
	End

	Event
		At Root
		Type Start
		CameraSpace
		Part WORLD\City\DarkAstoria\ScaryFlashes03.part
		LifeSpan 100
	End
End

#################################################################

End
