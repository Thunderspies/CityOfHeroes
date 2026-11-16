#############################################################
## MurderOfCrows_TeleportIn.bhvr
#############################################################

FxInfo
Lifespan 200

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0
	Random	1

	Event
		EName 	Swirls_Chest
		Type	Local
		At	Hips
		Part	:CallRavens_Dust_Swirling.part
		POther	Head
	End
End

Condition
	On	Time
	Time	0
	Random	1


	Event
		EName 	Crows_Chest
		Type	Local
		At	Chest
		Part	:CallRavens_Swirling.part
		Part	:CallRavens_Swirling2.part
		Part	:CallRavens_SwirlingX.part
		Part	:CallRavens_SwirlingX2.part

		PMagnet	Head
	End

	Event
		EName 	Crows_Hips
		Type	Local
		At	Hips
		Part	:CallRavens_Swirling.part
		Part	:CallRavens_Swirling2.part
		Part	:CallRavens_SwirlingX.part
		Part	:CallRavens_SwirlingX2.part

		PMagnet	Chest
	End
End

#############################################################

End