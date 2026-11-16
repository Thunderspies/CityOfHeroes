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
		EName 	Pack
		Type	local
		At	Chest
		Geom	FX_SkyRaiderJets  #jumpBotJet
		Bhvr	:FadeOut.bhvr
	End

		Event
		EName 	JetL
		Type	local 
		#Geom	FX_SkyRaiderJets
		At	Pack
		 AltPiv 1
		Bhvr	:FadeOut.bhvr
		#Part1	:JetPackThrusterGlowBlue.part
		#Part2	:ThrusterCenterBlue.part
		#Part3	:Thrustercore.part
		#Part4	:ThrusterTrail.part
		#Part5	:ThrusterGases.part
		#Part	:ThrusterGasesTrail.part
		Sound jetpack2_loop 70 70 .3
	End
End

End
##################################

