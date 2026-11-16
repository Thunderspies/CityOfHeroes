#############################################################
## PsionicShockwave_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FS_PsionicShockwave_Attack_01 100 100 .8
	End
End


Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/CameraShake_Subtle60.bhvr
		Lifespan	40
	End
End

Condition
	On	Time
	Time	42

	Event
		Type	PositOnly
		At	Chest
		Bhvr	behaviors/CameraShake_Strong20.bhvr
		Part	:PsionicShockwave_Flash_Initial.part
		Part	:PsionicShockwave_Globs.part
		Part	:PsionicShockwave_Globs_Alpha.part
		Lifespan	12
	End
End

Condition
	On	Time
	Time	44

	Event
		Type	PositOnly
		At	Chest
		Bhvr 	:PsionicShockwave_Bubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	150 50 255
			PyrRotateJitter	180.0 180.0 0.0
			FadeInLength	1
			FadeOutLength	10
			Spin 		0.5 0.5 0
		End
		Geom	Disintegration_Bubble
		Lifespan	8
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr 	:PsionicShockwave_Bubble.bhvr
		BhvrOverride
			Alpha		255
			StartColor	150 50 255
			PyrRotateJitter	-180.0 -180.0 0.0
			FadeInLength	1
			FadeOutLength	10
			Spin 		0.5 0.5 0
		End
		Geom	Disintegration_Bubble
		Lifespan	8
	End
End

Condition
	On	Time
	Time	47

	Event
		Type	PositOnly
		At	Chest
		Part	:PsionicShockwave_Glow.part
		Lifespan	12
	End

	Event
		Type	PositOnly
		At	Chest
		Part	:PsionicShockwave_Border.part
		Lifespan	10
	End
End

#############################################################

End