#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo
Lifespan 220
#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition

	On	Cycle
	Time	2

	Event
		EName	BeamTarget2
		Type	Local
		At	C_Hair
		BhvrOverride
			PositionOffset		0 5 5
			StartJitter		2 2 2
		End
		Lifespan 20
	End

	Event
		Ename	Spray
		Type	Local
		At 	C_Hair
		Part	:LightningSpray_RetinaBurn.part
		Part	:LightningSpray_Small.part
		POther	BeamTarget2
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	4

	Event
		EName	BeamTarget
		Type	Local
		At	C_Hair
		BhvrOverride
			PositionOffset		0 5 5
			StartJitter		2 2 2
		End
		Lifespan 20
	End

	Event
		Ename	Spray
		Type	Local
		At 	C_Hair
		Part	:LightningSpray_Curved.part

		POther	BeamTarget
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	8

	Event
		EName	BeamTarget
		Type	Local
		At	C_Hair
		BhvrOverride
			PositionOffset		0 5 5
			StartJitter		2 2 2
		End
		Lifespan 20
	End

	Event
		Ename	Spray
		Type	Local
		At 	C_Hair
		Part	:LightningSpray.part
		Part	:LightningSpray_RetinaBurn.part
		POther	BeamTarget
		Lifespan 1
	End
End


#############################################################################

End