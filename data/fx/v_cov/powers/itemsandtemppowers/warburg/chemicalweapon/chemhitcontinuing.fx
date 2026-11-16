#########################################################
##	Warburg Bomb - Nuke
##

FxInfo

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound miasma_loop 80 80 .33

	End
End

Condition

	On 	Time
	Time 	0

	Event
		EName   Splash
		Type	Local
		At	Waist
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemHitSplash.part
		part	:ChemSmoke.part
		#Lifespan	5
	End
	
	Event
		EName   Steam
		Type	Posit
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemSmoke02.part
		part	:ChemHitMist.part
		#Lifespan	15
	End

End

Condition
	On 	Cycle
	Time 	7
	Chance	.3
	
	Event
		EName   Drips
		Type	PositOnly
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemDrips.part
		Lifespan	5
	End

End

Condition
	On 	Cycle
	Time 	8
	Chance	.25
	
	Event
		EName   Drips
		Type	PositOnly
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemDrips.part
		Lifespan	5
	End

End

Condition
	On 	Cycle
	Time 	6
	Chance	.25
	
	Event
		EName   Drips
		Type	PositOnly
		At	LarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemDrips.part
		Lifespan	5
	End

End

Condition
	On 	Cycle
	Time 	7
	Chance	.25
	
	Event
		EName   Drips
		Type	PositOnly
		At	LarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemDrips.part
		Lifespan	5
	End

End

Condition
	On 	Cycle
	Time 	7
	Chance	.25
	
	Event
		EName   Drips
		Type	PositOnly
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemDrips.part
		Lifespan	5
	End

End

Condition
	On 	Cycle
	Time 	8
	Chance	.25
	
	Event
		EName   Drips
		Type	PositOnly
		At	HandR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemDrips.part
		Lifespan	5
	End

End

Condition
	On 	Cycle
	Time 	9
	Chance	.25
	
	Event
		EName   Drips
		Type	PositOnly
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemDrips.part
		Lifespan	5
	End

End

Condition
	On 	Cycle
	Time 	10
	Chance	.25
	
	Event
		EName   Drips
		Type	PositOnly
		At	UarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:ChemDrips.part
		Lifespan	5
	End

End

Condition
	On 	Cycle
	Time 	6
	Chance	.25

	Event
		EName   AcidRainDrop
		Type	Local
		At	Root
		Splat	Generic_Ring.tga
		Bhvr	:ChemRippleSplat.bhvr
		BhvrOverride
			StartJitter 1 0 1
		End
		Lifespan	5
	End

End

End
