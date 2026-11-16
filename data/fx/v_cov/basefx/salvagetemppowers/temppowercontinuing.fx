#########################################################
##	Fire_Ball
##
FxInfo
##################################

Condition
	On	Time
	Time	0
	
	Event
		Type	Local
		At	origin
		
		ChildFx	:FlightBoost.fx
		

	End

End

Condition
	On	Time
	Time	20
	
	Event
		EName	2
		Type	Local
		At	FootR
		Part	:Tendril.part
		Part	:BaseGlow.part
		

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		Part	:Tendril.part
		Part	:BaseGlow.part

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		Part	:Tendril.part
		Part	:BaseGlow.part
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		Part	:Tendril.part
		Part	:BaseGlow.part
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		Part	:Tendril.part
		Part	:BaseGlow.part

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		Part	:Tendril.part
		Part	:BaseGlow.part
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		Part	:Tendril.part
		Part	:BaseGlow.part
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		Part	:liteBaseGlow.part
		Part	:LiteTendril.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		Part	:liteBaseGlow.part
		Part	:LiteTendril.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		Part	:liteBaseGlow.part
		Part	:LiteTendril.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		Part	:liteBaseGlow.part
		Part	:LiteTendril.part
	End

End    


End	

