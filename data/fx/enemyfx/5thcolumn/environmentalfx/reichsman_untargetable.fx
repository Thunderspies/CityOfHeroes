#########################################################
##	forceBubble
########################################################
FxInfo

######################################################################################################3

Condition
	On 	Time
	Time 	0




######################################################################################################3




	Event
		Type	Local
		At	UArmR
		Part	:GlowQuick2.part
		Part	:ElectricBeam3.part
		PMagnet	Chest
	End

	Event
		Type	Local
		At	UArmR
		Part	:GlowQuick2.part
		Part	:ElectricBeam5.part
		PMagnet	UArmL
	End

	Event
		Type	Local
		At	LArmR
		Part	:ElectricBeam5.part
		PMagnet	UArmR
	End

	Event
		Type	Local
		At	UArmL
		Part	:GlowQuick2.part
		Part	:ElectricBeam3.part
		PMagnet	Chest
		Lifespan 70
	End

	Event
		Type	Local
		At	UArmL
		Part	:GlowQuick2.part
		Part	:ElectricBeam5.part
		PMagnet	UArmR
	End

	Event
		Type	Local
		At	LArmL
		Part	:ElectricBeam5.part
		PMagnet	UArmL
	End

	Event
		Type	Local
		At	LLegR
		Part	:ElectricBeam3.part
		PMagnet	ULegR
	End

	Event
		Type	Local
		At	LLegL
		Part	:ElectricBeam3.part
		PMagnet	ULegL
	End

	Event
		Type	Local
		At	FootR
		Part	:GlowQuick2.part
		Part	:ElectricBeam4.part
		POther	WepR
	End

	Event
		Type	Local
		At	FootR
		Part	:GlowQuick2.part
		Part	:ElectricBeam4.part
		POther	LLegR
	End

	Event
		Type	Local
		At	FootL
		Part	:GlowQuick2.part
		Part	:ElectricBeam4.part
		POther	WepL
	End

	Event
		Type	Local
		At	FootL
		Part	:GlowQuick2.part
		Part	:ElectricBeam4.part
		POther	LLegL
	End
End

######################################################################################################3

End
