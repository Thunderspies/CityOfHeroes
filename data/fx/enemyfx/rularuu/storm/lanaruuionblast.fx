#########################################################
##	
##

FxInfo

##################################




Condition
	On 	Time
	Time 	15

	Event
		EName 	Prime
		Type	start 
		At	WepL
		geom	CenterDummy
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part	:LanaruuStormIonPeachEnergy.part
		Part	:LanaruuStormIonDarkEnergy.part
		Part	:LanaruuStormIonBlueEnergy.part
		Part	:LanaruuStormIonBlueEnergy.part
		Part	:LanaruuStormIonBullet.part
		Part	:LanaruuStormIonBulletPurple.part
		Part	:LanaruuStormIonLightningTrail.part
		Part	:LanaruuStormIonLightningTrail2.part
		Part	:LanaruuStormIonLightningTrailBlack.part
		Part	:LanaruuStormIonLightningTrailBlack2.part
		Part	:LanaruuStormIonHead.part
		Sound lanaruionblast 180 180 1.0
		Magnet	Target
		LookAt	Target
		
	End

End


Condition
	On 	PrimeHit
	
	Event	
		ENAME	Lightning
		Type	Local
		At	T_Root
		BHVR	Behaviors/PivotOffsetChest.bhvr
		part	:AOE_Lightning_RING.part
		part	:AOE_Lightning_RINGDebris.part
		Part	:EnergyRingFlat.part
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	200

	Event
		Ename 	all
		Type	Destroy
	End

End

End				