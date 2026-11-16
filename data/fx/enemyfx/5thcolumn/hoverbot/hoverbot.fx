#########################################################
##	Fireweapon
########################################################
FxInfo

Flags    InheritAlpha

##################################################################
Condition

	Event
		Type	Local 
		Until	Death
		At	Origin
		Sound hoverbot2_loop 110 110 .45
	End

	
################################################
##Eyes
################################################

	Event
		EName 	P
		Type	Local 
		At	Hair
		Geom	FX_HoverBot
	End
	
	Event
		EName 	Eyez1bottem
		Type	local
		At	P
		 AltPiv 6
		Part	:HoverBotEyeGlow.part
		Part	:HoverBotEyeCenter.part	
		PMagnet Eyez1bottemMagnet		
	End 
	
	Event
		EName 	Eyez1bottemMagnet
		Type	local 
		At	P
		 AltPiv 9			
	End
	
	Event
		EName 	Eyez1topMagnet
		Type	local 
		At	P
		 AltPiv 8		
	End

	Event
		EName 	Eyez2top
		Type	local 
		At	P
		 AltPiv 7
		Part1	:HoverBotEyeGlow2.part
		Part2	:HoverBotEyeCenter2.part
		PMagnet Eyez1topMagnet		
	End


	Event
		EName 	Eyez1bottem2
		Type	local 
		At	P
		 AltPiv 6
		Part1	:HoverBotEyeGlow4.part
		Part2	:HoverBotEyeCenter4.part
		PMagnet Eyez1bottemMagnet2		
	End
	
	Event
		EName 	Eyez1bottemMagnet2
		Type	local 
		At	P
		 AltPiv 11		
	End
	
	Event
		EName 	Eyez1topMagnet2
		Type	local 
		At	P
		 AltPiv 10		
	End

	Event
		EName 	Eyez2top2
		Type	local 
		At	P
		 AltPiv 7
		Part1	:HoverBotEyeGlow3.part
		Part2	:HoverBotEyeCenter3.part
		PMagnet Eyez1topMagnet2	
	End

################################################
##wing lights
################################################

	Event
		EName 	WingLight1
		Type	local 
		At	P
		 AltPiv 4
		Part1	:HoverBotRedLightGlowBlue.part
		Part2	:HoverBotRedLightCenterBlue.part			
	End

	Event
		EName 	WingLight2
		Type	local 
		At	P
		 AltPiv 3
		Part1	:HoverBotRedLightGlowBlue.part
		Part2	:HoverBotRedLightCenterBlue.part			
	End

	Event
		EName 	Light2
		Type	local 
		At	P
		 AltPiv 5
		Part1	:HoverBotRedLightGlow2.part
		Part2	:HoverBotRedLightCenter2.part
		
	End

	Event
		EName 	Light1
		Type	local 
		At	Belt
		Part1	:HoverBotRedLightGlow.part
		Part2	:HoverBotRedLightCenter.part
	End

################################################
##thrusters
################################################

	Event
		EName 	Thruster2
		Type	local 
		At	FootL
		Part	:ThrusterGlowBlue.part
		Part	:ThrusterCenterBlue.part
		Part	:Thrustercore.part
		Part	:ThrusterTrail.part
		Part	:ThrusterGases.part	
	End

	Event
		EName 	Thruster1
		Type	local 
		At	FootR
		Part	:ThrusterGlowBlue.part
		Part	:ThrusterCenterBlue.part
		Part	:Thrustercore.part
		Part	:ThrusterTrail.part
		Part	:ThrusterGases.part
	End
End

End
##################################

