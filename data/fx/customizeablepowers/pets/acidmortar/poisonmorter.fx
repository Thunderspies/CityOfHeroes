#########################################################
##	forceBubble
########################################################
FxInfo

LifeSpan	200

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	archPoints
		Type	start
		At	root
		BHVR	V_COV\Powers\Traps\AllNOneLauncherScale.bhvr
		geom	electricityoffset

	End

	Event
		EName 	Prime
		Type	start
		At	archPoints
		altpiv	3

		Part	:AcidBurst.part
		Part	:AcidBurstDark.part

		Part	:AcidFlat.part
		Part	:AcidFlatDark.part
		Part	:AcidBubbles.part
		Part	:AcidBubblesdark.part
		Part	:AcidBubble.part

		Part	:Acid.part
		Part	:AcidYellow.part

		Geom	GEO_WepR_Grenade_1
		Bhvr	Behaviors/MorterLobbprojectile.bhvr
		Sound grenade1 80 80 .83

		Magnet	Target
		Lookat	Target

	End


End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End


	Event
		Type	start
		At	T_Root

		BHVR	Behaviors/RootToChestAdjustment.bhvr
		Part	:HitAcidDrops.part
		Part	:HitAcidDropsDark.part

		LifeSpan 8

	End


	Event
		Type	start
		At	T_Root

		Part	:AcidDrops.part
		Part	:AcidDropsDark.part

		LifeSpan 1

	End

	Event
		EName 	arrow
		Type	start
		At	T_Root

		Part	:AcidFlat2.part
		Part	:AcidFlatDark2.part
		Part	:AcidBubbles2.part
		Part	:AcidBubblesdark2.part
		Part	:AcidBubble2.part
		LifeSpan 20

	End

End

End