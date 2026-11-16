#############################################################
## SweepingRay_Attack.fx
#############################################################

FxInfo
Flags DontSuppress
Lifespan 60

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Anchor
		Type	PositOnly
		At	Hips
		BhvrOverride
			Scale		150 150 150
			PositionOffset	0 10 -140 #-120
			PYRrotate	0 0 0
		End
		Geom	FX_ConeOFfsets
	End

	Event
		EName	AnchorPiv
		Type	Start
		At	Anchor
		AltPiv	1
	End

	Event
		Type	PositOnly
		At	AnchorPiv
		Part	:Roar_Cone.part
		Part	:Roar_Air_Detached.part
		POther	HIps
		LookAt	HIps
	End
End



#############################################################

End