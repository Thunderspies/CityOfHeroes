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
		Sound Ionblast2 100 100 .89
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