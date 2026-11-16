#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Eyes
End

Input  
	InpName	Hips
End

Input  
	InpName	Root
End

Flags    InheritAlpha

########################################################
#Audio
########################################################

Condition
	On 	Time
	Time 	0

	Event

		Type	Local
		At	Hips
		Sound spectre_loop 55 55 .8
	End


End

########################################################
#Hips
########################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	Hips
		Part1	:daemon_hips.part
		#Part2	:FX_daemontenticle.part	
	End

	Event
		EName 	Tenticles
		Type	Local
		At	Hips
		Part1	:FX_daemontenticleLeft.part
		Part2	:FX_daemontenticleright.part
		#Part3	:daemon_hipsMove.part
	End


#######################################################


#Below Eyes

########################################################

	Event
		EName 	Prime
		Type	Local 
		At	Eyes
		Geom	FX_Daemon
	
	End

	Event
		Type	Local 
		At	Prime
		 AltPiv 1
		Part1	:DaemonPupil.part
		Part2	:DaemonEyes.part
		
	End
	
	Event
		Type	Local 
		At	Prime
		 AltPiv 2
		Part1	:DaemonPupil.part
		Part2	:DaemonEyes.part
		
	End

End

Condition
	On 	triggerbits
	Triggerbits	death

	
	Event
		EName 	OrbMist
		Type	Local 
		At	hips
		geom	FX_RuneStreak
		bhvr	behaviors/SpectrespinFast.bhvr
		part1	:DeamonDarkessChillMist.part
		part2	:DeamonDarkessChillMist2.part
						
	End	
	
	Event
		EName	SpectreDeathFX
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Geom	ChestOffset
		
	End

	Event
		
		Type	start
		At	hips
		Part2	:DeamonDeathCoreDark.part
		Part1	:DeamonDeathCore.part
		Part	:DeamonDeathRings.part
		
	End

	Event
		Type	start
		At	hips
		Bhvr 	Behaviors/SpectreDeathWisps.bhvr
		geom	FX_LevelupStreak
		LifeSpan	5
	End

	Event
		Type	start
		At	hips
		Bhvr 	Behaviors/SpectreDeathWisps2.bhvr
		geom	FX_LevelupStreak
		LifeSpan	5
	End

	Event
		Type	start
		At	Root
		Bhvr 	Behaviors/SpectreDeathWispsRoot.bhvr
		geom	FX_LevelupStreak
		LifeSpan	5
	End

	Event
		Type	start
		At	Root
		Bhvr 	Behaviors/SpectreDeathWispsRoot2.bhvr
		geom	FX_LevelupStreak
		LifeSpan	5
	End

	Event
		Ename	All
		Type	Destroy
				
	End

	
End

End
##################################

