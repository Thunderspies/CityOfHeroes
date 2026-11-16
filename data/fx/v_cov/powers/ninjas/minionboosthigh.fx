#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

#########################################################

Condition
	On 	Time
	Time 	0


	Event
		EName 	initialGlow
		Type	local
		At	chest
		
		ChildFx	V_COV\Powers\Ninjitsu\NinjaElude.fx	
						
	End

End

Condition
	On 	Time
	Time 	59


	Event
		EName 	Prime
		Type	Start 
		At	Root
		Bhvr	behaviors/slowprjct5.bhvr

		Part	:1FireCore.part
		part	:1FlatEvilSmoke.part
		part	:1FlatEvilSmoke2.part
		part	:1EvilSmoke.part
		part	:1EvilSmoke2.part
		Part	:1EvilTendril.part

		Magnet	T_Root
		LookAt	T_Root
						
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Splat
		Type	start 
		At	T_root
		
		Splat	Kuji1.tga
		BHVR	:Splat.bhvr

		LifeSpan	40
			
		
	End

	Event	
		Type	start
		At	T_Root
		BHVR	V_COV\Powers\Necromacy\Blacksplat.bhvr
		
		Splat	Ember.tga
		LifeSpan	15
	End

	Event
		EName 	Orb
		Type	Local 
		At	T_root
		Part	:1MaroonMagic.part
		part	:1HitEvilSmoke.part
		part	:1HitEvilSmoke2.part
#		part	:Sparks.part
		LifeSpan	50
			
		
	End

	Event
		EName 	cylinderspawn
		Type	local
		At	T_Root
		Geom	CenterDummy
		BHVR	V_COV/Powers/Poison/PoisonHealRotate.bhvr
		LifeSpan	40
	End

#################################################################################################

	Event
		EName 	cylinderspawn
		Type	local
		At	T_Root
		Geom	CenterDummy
		BHVR	V_COV\Powers\Poison\PoisonHealRotate.bhvr
		LifeSpan	30
	End
End

Condition
	On 	cycle
	Time	1

	Event
		EName	RotateNode
		Type	local
		At	cylinderspawn
		altpiv	1
		Bhvr	V_COV\Powers\Poison\PoisonHealRotate2.bhvr
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time	1
	Chance	.6

	Event

		Type	start
		At	RotateNode
		
		Part	:FireUp.part
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time	1
	Chance	.6

	Event

		Type	start
		At	RotateNode
		
		Part	:FireUp.part
		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time	1
	Chance	.6

	Event

		Type	start
		At	RotateNode
		
		Part	:FireUp.part
		LifeSpan	10

	End

End

End	


			