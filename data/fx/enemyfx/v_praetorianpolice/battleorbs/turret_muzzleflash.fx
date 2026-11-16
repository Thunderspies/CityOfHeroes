#############################################################
## Turret_MuzzleFlash.fx
#############################################################

FxInfo
LifeSpan 30

#############################################################

Condition
	On 	Cycle
	Time 	5

	Event
		EName	MuzzleAnchor
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 2.5 51.0
			PyrRotate	90 270 0
		End
		Part	:Flash_Beam.part
		Part	:Flash_Beam_Highlight.part
		Part	:Flash_Beam_Smoke.part
		Part	:Flash_Beam_light.part
		Lifespan	12
	End

	Event
		Type	Local
		At	MuzzleAnchor
		Part	:Flash_Circle.part
	#	Part	:Flash_Circle_Light.part
		Part	:Flash_Circle_Large.part
		Part	:Flash_Circle_Fire.part
		Lifespan	2
	End
End

#############################################################

End