#########################################################
##	jack
##
FxInfo


Input  
	InpName	Root
End

Flags    InheritAlpha


Condition
	On 	Time
	Time 	0

	Event
		EName 	particles
		Type	posit 
		At	Root
		Part	:MushroomPortalRingsDust.part
		Part	:MushroomPortalglowBase.part
		Part	:MushroomPortalRings.part
		Sound Mushroomportal_loop 80 80 .88
	End

#	Event
#		Type	local 
#		At	Root
#		BHVR	:Offset90.bhvr
#		Part	:MushroomPortalRings.part
#		Part	:MushroomPortalRingPulse.part
#		#Sound	Mushroomhit3_loop 80 70 .33
#	End

End

End			