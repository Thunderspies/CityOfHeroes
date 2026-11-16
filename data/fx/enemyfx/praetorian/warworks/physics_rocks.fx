#############################################################
## Physics_Sparks.fx
#############################################################

FxInfo
LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Chest

		Bhvr	behaviors\Explosion_Debris.bhvr
		BhvrOverride
			InitialVelocity		0.0 0.3 0.0
			InitialVelocityJitter	0.8 0.2 0.8
		End
		Part	:Physics_Rocks.part
	#	Part	:Physics_Trail_Smoke.part
		Lifespan	90
		LifespanJitter	30
		CThresh 	.01
		CDestroy 	0
	End

End

#############################################################

End
