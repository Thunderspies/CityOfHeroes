#########################################################
##	Fire_Ball
##
FxInfo
##################################

#Condition
#	On	Time
#	Time	0
#	
#	Event
#		Type	Local
#		At	origin
#		
#		ChildFx	:FlightBoost.fx
#		
#
#	End
#
#End

Condition
	On	Time
	Time	10
	
	Event
		EName	2
		Type	Local
		At	FootR
		Part	:CoreGlow.part
		Part	:litestTendril.part
		Part	:LightestPurpleBaseGlow.part
		

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		Part	:CoreGlow.part
		Part	:litestTendril.part
		Part	:LightestPurpleBaseGlow.part

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Part	:CoreGlow.part
		Part	:litestTendril.part
		Part	:LightestPurpleBaseGlow.part
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		Part	:CoreGlow.part
		Part	:litestTendril.part
		Part	:LightestPurpleBaseGlow.part
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		Part	:CoreGlow.part
		Part	:litestTendril.part
		Part	:LightestPurpleBaseGlow.part

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		Part	:CoreGlow.part
		Part	:litestTendril.part
		Part	:LightestPurpleBaseGlow.part
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		Part	:LiteCoreGlow.part
		Part	:litestTendril.part
		Part	:LightestPurpleBaseGlow.part
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		Part	:LiteCoreGlow.part
		Part	:LightestPurpleliteBaseGlow.part
		Part	:litestTendril.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		Part	:LiteCoreGlow.part
		Part	:LightestPurpleliteBaseGlow.part
		Part	:litestTendril.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		Part	:LiteCoreGlow.part
		Part	:LightestPurpleliteBaseGlow.part
		Part	:litestTendril.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		Part	:LiteCoreGlow.part
		Part	:LightestPurpleliteBaseGlow.part
		Part	:litestTendril.part
	End

End    


End	

