#########################################################
##	template

FxInfo


LifeSpan	70

Condition
	On 	Time
	Time 	14

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .8
	End

End


Condition
	On 	Time
	Time 	35

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Lifespan 1
	End


End
#############################################################
##projectile
#################################################

Condition
	On 	Time
	Time 	32

	Event
		EName	Prime
		Type	start
		At	FootR	

		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		
		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part

		Part	:MaceEnergyRings.part
		
		Bhvr	behaviors/MaceEnergyprojectile.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End

	
	Event
		EName	Prime1
		Type	start
		At	FootR	

		Part	:MaceEnergyRingsFlat.part
		Part	:MaceEnergyRings3.part
		Bhvr	behaviors/MaceEnergyprojectile.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	Prime2
		Type	start
		At	FootR	

		Geom	BattleAxe
		
		Bhvr	behaviors/BattleAxePrjct1.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End

	Event
		EName	Prime3
		Type	start
		At	FootR	

		Geom	BattleAxe

		Bhvr	behaviors/BattleAxePrjct1.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End

	Event
		EName	Prime4
		Type	start
		At	FootR	

		Geom	BattleAxe

		Bhvr	behaviors/BattleAxePrjct1.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End
End


Condition
	On 	Time
	Time 	41

	Event
		EName	streak
		Type	Destroy
		
	End
	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime1
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
	On 	Prime4Hit
	
	Event
		EName 	Prime4
		Type	Destroy

	End

End
	
End	
		

	