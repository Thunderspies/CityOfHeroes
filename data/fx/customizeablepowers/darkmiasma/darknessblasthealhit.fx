#############################################################
## DarknessBlastHealHit.fx
#############################################################

FxInfo
LifeSpan	300

#########################################################

Input
	Inpname	Hips
End

#########################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start
		At	chest
		Bhvr	CustomizeablePowers\DarkMiasma\slowprjct5.bhvr
		Magnet	t_chest
		LookAt	t_chest

		Part	CustomizeablePowers\DarkMiasma\Blast.part

		Part	CustomizeablePowers\DarkMiasma\DarkEmber.part
		Part	CustomizeablePowers\DarkMiasma\DarkEmberLarge.part
	End

	Event
		EName 	Prime2
		Type	Start
		At	chest
		Bhvr	CustomizeablePowers\DarkMiasma\slowprjct5.bhvr
		Magnet	t_chest
		LookAt	t_chest

		Part	CustomizeablePowers\DarkMiasma\Blast.part

		Part	CustomizeablePowers\DarkMiasma\DarkBlastTendrill01.part
		Part	CustomizeablePowers\DarkMiasma\DarkBlastTendrill03.part

	End

	Event
		EName 	Prime3
		Type	Start
		At	chest
		Bhvr	CustomizeablePowers\DarkMiasma\slowprjct5.bhvr
		Magnet	t_chest
		LookAt	t_chest

		Part	CustomizeablePowers\DarkMiasma\Blast.part

		Part	CustomizeablePowers\DarkMiasma\DarkBlastTendrill02.part
		Part	CustomizeablePowers\DarkMiasma\DarkBlastTendrill04.part

	End

	Event
		EName 	HitFake
		Type	local
		At	chest
		Part	CustomizeablePowers\DarkMiasma\Blast.part
		Part	CustomizeablePowers\DarkMiasma\Blast.part
		lifespan 10
	End
End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End
End


Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End


Condition
	On 	Prime3Hit
	Event
		EName 	Prime3
		Type	Destroy
	End

End

End


