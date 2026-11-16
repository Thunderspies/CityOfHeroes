#############################################################
## ElectricShackles_Continuing.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition

	On	Time
	Time	0

	Event
		Type	local
		At	Chest
		Part	:Nano_Glow_Lingering.part
		Part	:Nano_Rings_Chest.part
		Part	:Nano_Dots.part
		PMagnet	Chest
		Sound WW_NanoReconstruction_Hit 200 200 .5
		Lifespan 80
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	local
		At	SpadL
		Part	:Nano_Stream_Neck.part
		POther	Neck
		Lifespan 50

	End

	Event
		Type	local
		At	SpadR
		Part	:Nano_Stream_Neck.part
		POther	Neck
		Lifespan 50
	End


End

Condition
	On	Time
	Time	25




	Event
		Type	local
		At	LArmL
		Part	:Nano_Stream.part
		POther	UArmL
		Lifespan 50
	End


End

Condition
	On	Time
	Time	30

	Event
		Type	local
		At	LArmR
		Part	:Nano_Stream.part
		POther	UArmR
		Lifespan 50
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	local
		At	WepL
	#	Part	:Nano_Glow_Hands.part
		#Part	:Nano_Rings_Hands.part
		Part	:Nano_Dots_Hands.part
		Lifespan 45
	End

	Event
		Type	local
		At	ULegL
		Part	:Nano_Stream.part
		POther	Hips
		Lifespan 50
	End
End

Condition
	On	Time
	Time	40



	Event
		Type	local
		At	WepR
	#	Part	:Nano_Glow_Hands.part
	#	Part	:Nano_Rings_Hands.part
		Part	:Nano_Dots_Hands.part
		Lifespan 45
	End



	Event
		Type	local
		At	ULegR
		Part	:Nano_Stream.part
		POther	Hips
		Lifespan 50
	End
End

#############################################################

End


