#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan	200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		BHVR	Behaviors/OffsetEyes.bhvr
		ChildFX	POWERS\CosmicPowers\Peacebringer\ActivationEyes\ActivationEyes_Child.fx
		LifeSpan	30
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan	5
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	WepL
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan	5
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	mystic
		Bhvr	:QuickBlastProjectile.bhvr
		Part	:QuickBlastHeadCore.part
		Part	:QuickBlastCore.part
		Part	:QuickBlastCore.part
		Part	:QuickBlastCore.part
		Part	:QuickBlastPeaceEnergy2.part
		Part	:QuickBlastPeaceEnergy.part
		Part	:PeaceBlastSparks.part
		Part	:QuickBlastHeadStreaks.part
		Part	:QuickBlastHeadGlow.part
		Part	:QuickBlastHeadRing.part
		Part	:SolidQuickBlastCore.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On	PrimeHit
			
	Event
		Ename	FireBallHit
		Type	local
		At	target
		#Part	:BigFireExplosion.part
		#Part	:FireBallHitSpark.part
	End

	Event
		Ename	All
		Type	Destroy
	End
End

#############################################################

End		