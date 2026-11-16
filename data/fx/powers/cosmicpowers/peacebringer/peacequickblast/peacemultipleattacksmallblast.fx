#############################################################
## Peacebringer - Proton Scatter (Reverse Hit Effect)
#############################################################

FxInfo

LifeSpan	200

#############################################################

Condition
	On 	Time
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	T_WepR
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Sound PeaceMulti2 90 90 1.0
		LifeSpan	5
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	T_Wepl
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		LifeSpan	5
	End
End

Condition
	On 	Time
	Time 	2

	Event
		EName 	Prime
		Type	Start 
		At	T_mystic
		Bhvr	:QuickBlastProjectile4.bhvr
		Magnet	Chest
		LookAt	Chest
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadCore.part
		Part	:SmallBlastHeadGlow.part
		Part	:SmallBlastSparks.part
		Part	:SmallBlastHeadStreaks.part
	End
End

#############################################################

Condition
	On	PrimeHit
	
	Event
		EName	SwordFlash2
		Type	Local
		At	Chest
		Part	:BlastFlash.part
		Part	:BlastFlashTendrils.part
		Part	:QuickBlastHitStreaks.part
		Part	:PeaceBlastHitSparks.part
		Part	:SunFlareHitStreaks.part
		#Part	:LightFlareHitStreaks.part
		#Part	:PeaceBlastHitSpinners.part
		Sound Peacehit1 80 80 .8
		LifeSpan	10
	End

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End		