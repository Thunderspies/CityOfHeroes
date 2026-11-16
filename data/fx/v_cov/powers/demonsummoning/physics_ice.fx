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

		Bhvr	V_COV\PhysicsEnabled\Fire_Demon.bhvr
		BhvrOverride
			Scale		.2 .2 .2
			StartColor	180 230 255
			SpinJitter	20 30 0
		End
		geom	tintable_FX_IceWall_01
		geom	tintable_FX_IceWall_02
		geom	tintable_FX_IceWall_03
		Lifespan	90
		LifespanJitter	30

		CEvent 	:Physics_Ice_CEvent.fx
		CThresh 	.001
		CDestroy 	1
	End

End

#############################################################

End
