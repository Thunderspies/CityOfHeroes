#############################################################
## Afterburner_Activation.fx
#############################################################

FxInfo

#############################################################


Condition
	On		TRIGGERBITS
	TRIGGERBITS	FORWARD
	DoMany		1

	Event
		EName	ForwardAir
		Type	Local
		At	Chest
		While	FORWARD
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0 3 1.5
		End
		Part	:Shockwave_Head_Continuing.part
		Part	:Shockwave_Continuing.part
		Part	:Shockwave_Head_Stream.part
		Lifespan 20
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	JUMP
	DoMany		1

	Event
		EName	ForwardAir
		Type	Local
		At	Chest
		While	JUMP
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0 3 1.5
		End
		Part	:Shockwave_Head_Continuing.part
		Part	:Shockwave_Continuing.part
		Part	:Shockwave_Head_Stream.part
		Lifespan 20
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	AIR
	DoMany		1

	Event
		EName	ForwardAir
		Type	Local
		At	Chest
		While	AIR
		Flags	OneAtATime
		BhvrOverride
			PositionOffset	0 3 1.5
		End
		Part	:Shockwave_Head_Continuing.part
		Part	:Shockwave_Continuing.part
		Part	:Shockwave_Head_Stream.part
		Lifespan 20
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	BACKWARD
	DoMany		1

	Event
		EName	ForwardAir
		Type	Local
		At	Chest
		While	BACKWARD
		Flags	OneAtATime
		Part	:Shockwave_Head_Continuing.part
		Part	:Shockwave_Continuing.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPLEFT
	DoMany		1

	Event
		EName	ForwardAir
		Type	Local
		At	Chest
		While	STEPLEFT
		Flags	OneAtATime
		Part	:Shockwave_Head_Continuing_Side.part
		Part	:Shockwave_Continuing_Side.part
	End
End

Condition
	On		TRIGGERBITS
	TRIGGERBITS	STEPRIGHT
	DoMany		1

	Event
		EName	ForwardAir
		Type	Local
		At	Chest
		While	STEPRIGHT
		Flags	OneAtATime
		Part	:Shockwave_Head_Continuing_Side.part
		Part	:Shockwave_Continuing_Side.part
	End
End

#############################################################

End