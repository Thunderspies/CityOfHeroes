#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Chest
End 

Flags    InheritAlpha

#Flags	 IsArmor

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	local
		At	Chest
		
		Bhvr	Behaviors/GenericParticleFade3.bhvr
		Geom	FX_SkyRaiderJets
		
	End

	Event
		Type	local
		At	Prime
		 AltPiv 3	

		Bhvr	Behaviors/GenericParticleFadeQuick.bhvr
		Part1	:WingLightStrip.part
		PMagnet magnet1
	End

	Event
		Type	local
		At	Prime
		 AltPiv 5	 

		Bhvr	Behaviors/GenericParticleFadeQuick.bhvr
		Part1	:WingLightStrip.part
		PMagnet magnet2
	End

End

#############################################################################

Condition
	On		TriggerBits
	TriggerBits	
	DoMany		1

	Event
		EName 	magnet1
		Type	local
		At	Prime
		 AltPiv 4
		Until	FLY
		Flags	OneAtATime

		Part1	:Smoke.part
		#Part2	:WingLightCenterBlue.part
		
	End

	Event
		EName 	magnet2
		Type	local
		At	Prime
		 AltPiv 6
		Until	FLY
		Flags	OneAtATime

		Part1	:Smoke.part
				
	End

	
End

###############################################################################################
########################################   Hover    ###########################################
###############################################################################################


###############################################################################################
########################################   In Air   ###########################################
###############################################################################################


Condition
	On	TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName 	JetL
		Type	local 
		At	Prime
		 AltPiv 1
		While	FLY 
		Flags	OneAtATime
		
		Part1	:JetPackThrusterGlowBlue.part
		Part2	:JetPackThrusterCenterBlue.part
		Part3	:JetPackThrustercore.part
		Part4	:JetPackThrusterTrail.part
		Part5	:JetPackThrusterGases.part
		Sound jetpack2_loop 70 70 .3
	End

	Event
		EName 	JetR
		Type	local
		At	Prime
		 AltPiv 2
		While	FLY 
		Flags	OneAtATime

		Part1	:JetPackThrusterGlowBlue.part
		Part2	:JetPackThrusterCenterBlue.part
		Part3	:JetPackThrustercore.part
		Part4	:JetPackThrusterTrail.part
		Part5	:JetPackThrusterGases.part
		Sound glow7_loop 70 70 .66
		
	End

End


End
##################################

