#########################################################
##	forceBubble
########################################################
FxInfo

######################################################################################################

Condition
	On 	Cycle
	Time 	38
	Chance	.75


	Event
		Type	Local
		At	LArmR
		Part	:ElectricBeam5.part
		Part	:ElectricBeamQuick_Small.part

		PMagnet	UArmR
		Lifespan 10
	End


	Event
		Type	Local
		At	LLegR
		Part	:ElectricBeam3.part
		Part	:ElectricBeamQuick_Small.part

		PMagnet	ULegR
		Lifespan 10
	End

	Event
		Type	Local
		At	WepL
		Part	:GlowQuick_Body.part
		Part	:ElectricBeam4.part
		Part	:ElectricBeamQuick_Small.part

		POther	LArmL
		Lifespan 5
	End
End

######################################################################################################

Condition
	On	Cycle
	Time	17
	Chance	.6

	Event
		Type	Local
		At	UArmL
		Part	:GlowQuick_Body.part
		Part	:ElectricBeam5.part
		Part	:ElectricBeamQuick_Small.part

		PMagnet	UArmR
		Lifespan 5

	End

	Event
		Type	Local
		At	WepR
		Part	:GlowQuick_Body.part
		Part	:ElectricBeam4.part
		Part	:ElectricBeamQuick_Small.part

		POther	LArmR
		Lifespan 5

	End

	Event
		Type	Local
		At	FootL
		Part	:GlowQuick_Body.part
		Part	:ElectricBeam4.part
		Part	:ElectricBeamQuick_Small.part

		POther	LLegL
		Lifespan 5

	End
End

Condition
	On	Cycle
	Time	24
	Chance	.25

	Event
		Type	Local
		At	UArmR
		Part	:GlowQuick_Body.part
		Part	:ElectricBeam5.part
		Part	:ElectricBeamQuick_Small.part

		PMagnet	UArmL
		Lifespan 5

	End



	Event
		Type	Local
		At	LArmL
		Part	:ElectricBeam5.part
		Part	:ElectricBeamQuick_Small.part

		PMagnet	UArmL
		Lifespan 15

	End



	Event
		Type	Local
		At	FootR
		Part	:GlowQuick_Body.part
		Part	:ElectricBeam4.part
		Part	:ElectricBeamQuick_Small.part

		POther	LLegR
		Lifespan 10

	End
End


Condition
	On	Cycle
	Time	30
	Chance	.65

	Event
		Type	Local
		At	LLegL
		Part	:ElectricBeam3.part
		Part	:ElectricBeamQuick_Small.part

		PMagnet	ULegL
		Lifespan 7

	End

	Event
		Type	Local
		At	UArmL
		Part	:GlowQuick_Body.part
		Part	:ElectricBeam3.part
		Part	:ElectricBeamQuick_Small.part

		PMagnet	Chest
		Lifespan 5
	End

End
######################################################################################################3

End
