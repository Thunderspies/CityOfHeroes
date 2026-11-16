#############################################################
## CoL_LightStaff.fx
#############################################################

FxInfo
Lifespan 300
#########################################################

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	CoL_LightStaffBolt_Attack_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		EName 	KeetsiePwns1
		Type	Local
		At	WepR
		Geom	CoL_Staff_01
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	KeetsiePwns2
		Type	Local
		At	KeetsiePwns1
		altpiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:StaffEnergyDots_Soul.part
		Flags	Oneatatime
                While   WEAPON
		lifespan 8
	End
End

Condition
	On	Time
	Time	13

#	Event
#		EName 	KeetsiePwns3
#		Type	Local
#		At	WepR
#		Geom	CoL_Staff_01
#		Flags	Oneatatime
#                While   WEAPON
#	End

	Event
		EName 	KeetsiePwns4
		Type	Local
		At	KeetsiePwns1
		altpiv	1
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:ProjectileInitFlash_Soul.part
		Flags	Oneatatime
                While   WEAPON
	End
End

Condition
	On 	Time
	Time 	22


	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	behaviors\Braz\Projectile_FastWithUp_01.bhvr
		Part	:Beam_Soul.part
		Part	:Beam_EdgeNoise_Soul.Part
		Part	:Beam_Particles_Soul.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Prime2
		Type	Destroy
	End

	Event
		EName 	Prime3
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	50

	Event
		EName 	Hand1
		Type	Destroy
	End

	Event
		EName 	Hand2
		Type	Destroy
	End
End

#########################
End