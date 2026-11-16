#########################################################
## Hamidon - Beam Hit FX
#########################################################

FxInfo

Lifespan	180
#Flags    InheritAlpha

#########################################################

Condition

	On	Time
	Time	0

	Event
		EName 	Hookup
		Type	Local 
		At	Root
		Geom	RootToChestAdjustment
		Sound mitohit5 60 60 .75
		
	End

	Event
		EName 	Flash
		Type	Local 
		At	Hips
		Part	:Hit_Flash.part
		Lifespan	7
		
	End	

	Event
		EName 	Flash_Decal
		Type	Local 
		At	Root
		Part	:Hit_Flash_Decal.part
		Lifespan	7
	End

	Event
		EName 	SplashRing
		Type	Local 
		At	Hookup
		Altpiv	1
		Part2   :Hit_Splash_Ring1.part
		#Part1   :Hit_Splash_Ring2.part
	End

End

#########################################################

End