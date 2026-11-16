#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Chest
End 

Flags    InheritAlpha


########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	local
		At	Chest
		Geom	PorterPac
		Sound SKyPorter_loop 80 80 .55
	
	End
	
	Event
		EName 	JetL
		Type	local 
		At	Prime
		 AltPiv 1
		Part1	:PorterLightBlue.part
		Part	:PorterLightBlueCore.part
		
	End

End

Condition
	On 	Time
	Time 	10

	Event
		EName 	JetR
		Type	local
		At	Prime
		 AltPiv 2
		Part1	:PorterLightBlue.part
		Part	:PorterLightBlueCore.part
		
		
	End

	Event
		EName 	JetR
		Type	local
		At	Prime
		 AltPiv 3
		Part1	:PorterLightBlueLarge.part
		Part	:PorterLightBlueCoreLarge.part
		Part	:PorterGlow01.part
		
	End

	Event
		EName 	magnet1
		Type	local
		At	Prime
		 AltPiv 4
		Part1	:PorterLightBlueLarge.part
		Part	:PorterLightBlueCoreLarge.part
		Part	:PorterGlow01.part
	End

	Event
		EName 	JetR
		Type	local
		At	Prime
		 AltPiv 5
		Part1	:PorterLightBlueLarge.part
		Part	:PorterLightBlueCoreLarge.part
		Part	:PorterGlow01.part
	End

	Event
		EName 	magnet2
		Type	local
		At	Prime
		 AltPiv 6
		Part1	:PorterLightBlueLarge.part
		Part	:PorterLightBlueCoreLarge.part
		Part	:PorterGlow01.part		
	End

End

End
##################################

