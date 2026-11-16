#############################################################
## Peacebringer - Incandescent Strike
#############################################################

FxInfo

LifeSpan	150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		BHVR	Behaviors/OffsetEyes.bhvr
		ChildFX	POWERS\CosmicPowers\Peacebringer\ActivationEyes\ActivationEyes_Child.fx
		LifeSpan	150
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	wepR
		Part	:HeavyPunchFlash.part
		Part	:HeavyPunchTendrils.part
		Part	:HeavyPunchMistEnergy.part
		Sound PeaceHands3 70 70 .8
		LifeSpan	100
	End

	Event
		Type	Local
		At	wepL
		Part	:HeavyPunchFlash.part
		Part	:HeavyPunchTendrils.part
		Part	:HeavyPunchMistEnergy.part
		Sound PeaceHeavyPunch 80 80 .8
		LifeSpan	100
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	:FootStompCameraShake.bhvr
		#Part	:EnergyCracksFlat.part
		LifeSpan	10
	End

	Event
		Type	Local 
		At	root
		Bhvr	:FootStompCameraShake.bhvr
		Part	:NovaTendrils.part
		Part	:NovaTendril.part
		Part	:NovaTendrilsFlat.part
		Part	:NovaTendrilFlat.part
		Part	:InfernorockBits2.part
		Part	:NovaTendrilBase.part
		LifeSpan	25
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		12
			PhysForcePower		300
			PhysForcePowerJitter	50
		End
		Lifespan	1
	End
End

#############################################################

End		