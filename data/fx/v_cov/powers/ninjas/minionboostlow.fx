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
		
		ChildFx	V_COV\Powers\Ninjitsu\NinjaPsionicsDefence.fx	
						
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

		Part	:FireCore.part
		part	:FlatEvilSmoke.part
		part	:FlatEvilSmoke2.part
		part	:EvilSmoke.part
		part	:EvilSmoke2.part
		Part	:EvilTendril.part

		Magnet	T_Root
		LookAt	T_Root
						
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Splat
		Type	start 
		At	T_root
		
		Splat	Kuji2.tga
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

#################################################################################################
	
	Event
		EName 	OrbMist
		Type	Local 
		At	T_Root
		geom	FX_RuneStreak
		bhvr	:SpinFast.bhvr
		LifeSpan	15
	End

	Event
		EName 	OrbStreak
		Type	Local 
		At	T_Root
		geom	FX_RuneStreak
		bhvr	:SpinFast2.bhvr
				
		LifeSpan	15
	End

	Event
		EName 	Orb
		Type	Local 
		At	OrbStreak
		altpiv  1
		Part	:MaroonMagic.part
		part	:HitEvilSmoke.part
		part	:HitEvilSmoke2.part
		Part	:MaroonMagic.part
		part	:HitEvilSmoke.part
		part	:HitEvilSmoke2.part
#		part	:Sparks.part
		
		
	End

	Event
		EName 	Orb
		Type	Local 
		At	OrbMist
		altpiv  1
		Part	:MaroonMagic.part
		part	:HitEvilSmoke.part
		part	:HitEvilSmoke2.part
		Part	:MaroonMagic.part
		part	:HitEvilSmoke.part
		part	:HitEvilSmoke2.part
#		part	:Sparks.part
			
		
	End
	
	Event
		EName 	OrbStreak
		Type	Local 
		At	T_Root
		
#		Bhvr	:OverHeadOffset.bhvr
		part	:Sparks.part
				
		LifeSpan	20
	End

	Event
		EName 	Prime
		Type	Destroy

	End

End


End	


			