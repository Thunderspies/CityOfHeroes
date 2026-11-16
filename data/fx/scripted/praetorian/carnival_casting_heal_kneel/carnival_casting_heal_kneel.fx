#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time	0

#########################################################
####################### AUDIO ###########################
#########################################################


	Event
		Type	Local
		At	Root
		Sound 	FW_Carnival_Casting_HealKneel_Loop 100 100 .1
	End


#############################################################

	Event
		Type	Local
		At	WepL
		Part	:Beacon_Border_Small.part
		Part	:Beacon_Swirls_Small.part
		Part	:Beacon_Swirls_Additive_Small.part
		Part	:Beacon_Core_Small.part
		Part	:Beacon_LightRays.part
		Part	:Beacon_Dots.part
	End

	Event
		Type	Local
		At	WepR
		Part	:Beacon_Border_Small.part
		Part	:Beacon_Swirls_Small.part
		Part	:Beacon_Swirls_Additive_Small.part
		Part	:Beacon_Core_Small.part
		Part	:Beacon_LightRays.part
		Part	:Beacon_Dots.part
	End
End




#########################################################

End