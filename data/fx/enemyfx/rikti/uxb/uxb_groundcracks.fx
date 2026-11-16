#########################################################
## UXB Ground Cracks
#########################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	crack
		Type	start
		At	Root
		ChildFX	ENEMYFX\Rikti\Shuttle\FakeBombs\Bomb_Spawn.fx
	End
End

#########################################################

Condition
	On 	Time
	Time 	59

	Event
		EName	crack
		Type	start
		At	Root
		Bhvr	:UXB_GroundCracks.bhvr
		BhvrOverride
		End
		Splat	ImpactCracks.tga
	End

	Event
		Type	Start
		At	Origin
		Bhvr	Behaviors/CameraShake01.bhvr
		BhvrOverride
			Shake		0.5	#How much to shake the camera when event with this bhvr is created
			ShakeFallOff	0.025	#how long shake lasts
			ShakeRadius	32	#(falls off slowly, so at 18.0 feet from center, only a mild shake)
		End
	End
End

#########################################################

End