#############################################################
## RadiationBlastAim.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Posit
		At	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	:EnergyCore.part
		Part	:EnergyCoreLightstreaks.part
		Part	:EnergyCoreLightStreaks2.part
		Part	:EnergyCoreGlowStreaks.part
		LifeSpan 38
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	ElectronRig
		Type	Local
		At	chest
		Bhvr	CustomizeablePowers\RadiationBlast\ElectronScale.bhvr
		Anim	FX_dark2
		LifeSpan 60
	End

	Event
		EName	Punch1
		Type	Local
		At	ElectronRig
		AnimPiv	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		LifeSpan 38
	End


	Event
		EName	Punch2
		Type	Local
		At	ElectronRig
		AnimPiv	Neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		LifeSpan 38
	End

	Event
		EName	Punch3
		Type	Local
		At	ElectronRig
		AnimPiv	Waist
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		LifeSpan 38
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Ename	ElectronRig3
		Type	Local
		At	chest
		Bhvr	CustomizeablePowers\RadiationBlast\ElectronScale3.bhvr
		Anim	FX_dark2
		LifeSpan 60
	End

	Event
		EName	Punch1
		Type	Local
		At	ElectronRig3
		AnimPiv	Chest
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		LifeSpan 38
	End


	Event
		EName	Punch2
		Type	Local
		At	ElectronRig3
		AnimPiv	Neck
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		LifeSpan 38
	End

	Event
		EName	Punch3
		Type	Local
		At	ElectronRig3
		AnimPiv	Waist
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:Electron.part
		Part2	:ElectronLocal.part
		LifeSpan 38
	End
End

## GENERIC FX ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		ChildFX	POWERS\Child_Aim.fx
	End
End

#############################################################

End