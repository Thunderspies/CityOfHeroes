#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	InpName chest
End

Input  
	Inpname	Belt
End

Input  
	InpName	Hips
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
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
		At	chest
		Sound daemon_loop 66 66 .8
		
	End

End



########################################################
#Hips
########################################################




Condition
	On 	Time
	Time 	0

	Event
		
		Type	Local
		At	wepR
		
		Part1	:HandGlow.part
		
	End

	Event
		
		Type	Local
		At	wepL
		
		Part1	:HandGlow.part
		
	End

	Event
		EName 	geo
		Type	Local
		At	Root
		
		Geom	ChestOffset
		
	End

	Event
		
		Type	Local
		At	geo
		
		AltPiv	1
		Part1	:BeltGlow.part
		Part2	:BeltGlow2.part
		
	End


	Event
		
		Type	Local
		At	Belt
		
		Part2	:BeltGlowLocal.part
	End



	Event
		EName 	flash
		Type	Local
		At	Hips
		
		Part1	:smoke.part
		Part4	:SpectreRips.part
		Part5	:SpectreRips.part
		Part5	:SpectreRips.part
		Part5	:SpectreRips.part
	End

	

	

	Event
		EName 	flash
		Type	Local
		At	Hips
		
		Part	:SpectreRipsLocal.part
		Part	:SpectreRipsLocal.part
		Part	:SpectreRipsLocal.part
		Part	:SpectreRipsLocal.part
		Part	:SpectreRipsLocal.part
		Part	:SpectreRipsLocal.part
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
		part1	:DarkessChillMist.part
		part2	:DarkessChillMist2.part
						
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
		Part2	:DeathCoreDark.part
		Part1	:DeathCore.part
		Part	:DeathRings.part
		
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

