#########################################################
## Umbral Aura - Inky Aspect
#########################################################

FxInfo

## SOUND FX #######################################################

Condition

	On	Time
	Time	0
	
	Event
		Type	Local 
		At	Head
		Sound WarHold2_loop 70 70 .3
		Sound WarShadeShield_loop 70 70 .88
	End
End

Condition

	On	Time
	Time	50
	
	Event
		Type	Local 
		At	Eyes
		Sound TransBurst 70 70 .45
	End
End

## EYE FX ######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		EName 	Eyes
#		Type	Local 
#		At	Root
#		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyesContinuingFX.fx
#	End
#End

## POWER FX #######################################################

Condition

	Event
		Type	Local 
		At	Root
		#BHVR	:OppressizeGloom.bhvr

		Part	:Gloom.part
		Part	:TendrilPurpleGloom.part
		Part	:TendrilBlueGloom.part
		Part	:TendrilGloom.part
		Pmagnet	Chest
	End

	Event
		Type	Local 
		At	Chest
		
		Part	:RedTendrils.part
		Part	:RedRing.part
		Part	:RedRingExpand.part
	End
End

#########################################################

End