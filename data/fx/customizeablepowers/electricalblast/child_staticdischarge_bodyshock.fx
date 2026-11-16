#############################################################
## Child_StaticDischarge_Bodyshock.fx
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Type	Local
		At	UarmR
		Part	:mu_Elec_BodyShock1.part
		Part	:mu_Elec_BodyShock2.part
		POther	LarmR
		PMagnet UarmR
	End

	Event
		Type	Local
		At	LarmR
		Part	:mu_Elec_BodyShock1.part
		Part	:mu_Elec_BodyShock2.part
		POther	HandR
		PMagnet LarmR
	End

	Event
		Type	Local
		At	UarmL
		Part	:mu_Elec_BodyShock1.part
		Part	:mu_Elec_BodyShock2.part
		POther	LarmL
		PMagnet UarmL
	End

	Event
		Type	Local
		At	LarmL
		Part	:mu_Elec_BodyShock1.part
		Pother	HandL
		PMagnet LarmL
	End

	Event
		Type	Local
		At	UlegR
		Part	:mu_Elec_BodyShock1.part
		Part	:mu_Elec_BodyShock2.part
		POther	LlegR
		PMagnet UlegR
	End

	Event
		Type	Local
		At	LlegR
		Part	:mu_Elec_BodyShock1.part
		Part	:mu_Elec_BodyShock2.part
		POther	FootR
		PMagnet LlegR
	End

	Event
		Type	Local
		At	UlegL
		Part	:mu_Elec_BodyShock1.part
		Part	:mu_Elec_BodyShock2.part
		POther	LlegL
		PMagnet UlegL
	End

	Event
		Type	Local
		At	LlegL
		Part	:mu_Elec_BodyShock1.part
		Part	:mu_Elec_BodyShock2.part
		POther	FootL
		PMagnet LlegL
	End
End

#############################################################

End