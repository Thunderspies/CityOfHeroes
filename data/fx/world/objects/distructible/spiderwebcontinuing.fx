#########################################################
##	spiderweb continuing 
########################################################
FxInfo

##################################################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	local
#		At	root
#		Sound	webgrexplo2 88 70 .95
#	End
#End	


Condition
	On 	Time
	Time 	0

#	Event
#		EName	webcage
#		Type	posit
#		At	root		
#		Geom	netwebbingsmallCage
#				
#	End

	Event
		Type	local
		At	head
		Part	:web.part
		magnet	Neck		
	End

#	Event
#		Type	local
#		At	neck
#		Part	:web.part
#				
#	End

	Event
		Type	local
		At	UarmR
		Part	:web.part
		magnet	LarmL		
	End

	Event
		Type	local
		At	UarmL
		Part	:web.part
		magnet	LarmL		
	End

	Event
		Type	local
		At	LarmR
		Part	:web.part
		magnet	HandR		
	End

	Event
		Type	local
		At	LarmL
		Part	:web.part
		magnet	HandL		
	End

	Event
		Type	local
		At	handR
		Part	:web.part
				
	End

	Event
		Type	local
		At	HandL
		Part	:web.part
				
	End

	Event
		Type	local
		At	chest
		Part	:web.part
		magnet	Hips		
	End

#	Event
#		Type	local
#		At	hips
#		Part	:web.part
#				
#	End

	Event
		Type	local
		At	ULegR
		Part	:WebLegs.part
		magnet	LlegR		
	End

	Event
		Type	local
		At	ULegL
		Part	:WebLegs.part
		magnet	LlegL		
	End

	Event
		Type	local
		At	LLegR
		Part	:WebLegs.part
		magnet	FootR		
	End

	Event
		Type	local
		At	LLegL
		Part	:WebLegs.part
		magnet	FootL		
	End

	Event
		Type	local
		At	FootR
		Part	:WebLegs.part
				
	End

	Event
		Type	local
		At	FootL
		Part	:WebLegs.part
				
	End

End

End