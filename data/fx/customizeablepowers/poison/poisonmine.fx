#########################################################
##	forceBubble
########################################################
FxInfo

#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	minepivot
		Type	start
		At	root
		Sound poisonmine_loop 18 18 .33
		Geom	gadgetHookup
		#Bhvr	behaviors/mineFade.bhvr
	End

	Event
		EName	mine
		Type	start
		At	minepivot
		altpiv	1
		Sound air_loop 18 18 .22

		Bhvr	V_COV\Powers\Poison\GreenGlassMineScale.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_PoisonGlassBall
	End
End


Condition
	On 	Cycle
	Time 	30

	Event
		EName	hk
		Type	start
		At	minepivot
		altpiv	1
		Bhvr	V_COV\Powers\Poison\splatElipse.bhvr
		BhvrOverride
			StartColor 	255 255 255
			PrimaryTint	80
			SecondaryTint	20
		End
		splat	Generic_Ellipse.tga
		Lifespan	10
	End
End

Condition
	On 	Cycle
	Time 	13
	Chance	.2

	Event
		EName	hk
		Type	start
		At	minepivot
		altpiv	1

		Bhvr	V_COV\Powers\Poison\splatElipse.bhvr
		BhvrOverride
			StartColor 	255 255 255
			PrimaryTint	80
			SecondaryTint	20
		End
		splat	Generic_Ellipse.tga
		Lifespan	10
	End
End

Condition
	On 	Cycle
	Time 	13
	Chance	.62

	Event
		EName	hk
		Type	start
		At	minepivot
		altpiv	1
		BhvrOverride
			StartColor 	255 255 255
			PrimaryTint	80
			SecondaryTint	20
		End
		Bhvr	V_COV\Powers\Poison\splat.bhvr
		splat	Generic_Ring.tga
		Lifespan	10
	End
End

End