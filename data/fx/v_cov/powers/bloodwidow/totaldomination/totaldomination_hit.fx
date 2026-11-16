#############################################################
## Blood Widow "Telekinetic Blast" Hit FX
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	POWERS\MentalPowers\HeadDizzies.part
		Part2	POWERS\MentalPowers\HeadDizzyRings.part
		part3	POWERS\MentalPowers\MindGlowHit2.part
		Sound mentalhit 80 80 .8
	End
End

#############################################################

End