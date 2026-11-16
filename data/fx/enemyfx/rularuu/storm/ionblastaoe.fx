#########################################################
##	
##

FxInfo

##################################

Condition
	On 	Time
	Time 	65

	Event
		Ename	Flash
		Type	local
		At	chest
		#Part1	WeaponFx\BurstStreaks.part
		#Part2	WeaponFx\BurstEmbers.part
		
		
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start 
		At	chest
		geom	CenterDummy
		Bhvr	Behaviors\Fastprojectile3.bhvr
		Part	:StormIonPeachEnergy.part
		Part	:StormIonDarkEnergy.part
		Part	:StormIonBlueEnergy.part
		Part	:StormIonBlueEnergy.part
		Part	:StormIonBullet.part
		Part	:StormIonBulletPurple.part
		Part	:StormIonLightningTrail.part
		Part	:StormIonLightningTrail2.part
		Part	:StormIonLightningTrailBlack.part
		Part	:StormIonLightningTrailBlack2.part
		Part	:StormIonHead.part
		Sound Ionblast1 100 100 .89
		Magnet	Target
		LookAt	Target
		
	End

End


Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End
	
	Event	
		ENAME	Lightning
		Type	Local
		At	T_chest
		
		Part	:EnergyRing.part
		LifeSpan	10
	End

	Event	
		ENAME	Lightning
		Type	Local
		At	T_chest
		part	:Storm_Lightning_01.part
		part	:Storm_Lightning_Flash_01.part
		part	:WhiteFlash.part
		LifeSpan	10
	End			
	
	Event	
		ENAME	Lightning
		Type	Local
		At	T_chest
		part	:Storm_Lightning_02.part
		part	:Storm_Lightning_Flash_01.part
		LifeSpan	10
	End			

	Event	
		ENAME	Lightning
		Type	Local
		At	T_chest
		part	:Storm_Lightning_03.part
		LifeSpan	10
	End			

	Event	
		ENAME	Cloud
		Type	Local
		At	T_chest
		part	:Storm_Heart_01.part
		part	:Storm_Center_01a.part
		part	:Storm_Center_02a.part
		part	:Storm_Center_01a.part
		part	:Storm_Center_02a.part
		LifeSpan	5
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