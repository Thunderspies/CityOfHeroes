
#########################################################
##	
##
FxInfo

Input  
	InpName	CHEST
End

Input  
	InpName	Origin
End	
	

Condition
	On 	triggerbits
	Triggerbits	death
	
################ If you want the guy to turn invisible right as he dies AND play particles, do this on triggerbits death
################ (you can't set TicksToLingerAfterDeath to less than about 15 if you want to have a 
################ deathFx because he'll be dead before the fx happens.)
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/vanish.bhvr
		Flags	AdoptParentEntity
	End

	Event
		EName 	Prime
		Type	Destroy
	End
#################### End do this

	
	Event

		EName	Gas
		Type	start
		At	chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:CrystalScatter.part		
		Part	:CrystalScatter2.part
		Part	:CrystalScatter.part		
		Part	:CrystalScatter2.part
#		Part	:PurpleCrystalScatter01.part		
#		Part	:PurpleCrystalScatter02.part
#		Part	:PurpleCrystalScatter01.part		
#		Part	:PurpleCrystalScatter02.part


		#Sound	poisonhit2 50 30 .7
		LifeSpan	30
		
	End
	
	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End

	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End

	
	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End

	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End

	
	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End

	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End

	
	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End

	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS2.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


	Event
		Type	Start 
		At	origin
		Bhvr	Behaviors\CrystalShatter.bhvr
		BhvrOverride

			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			Drag			0.004
			physics 		1
			physRadius 		.3
			
			PhysDebris		1
			PhysDensity		2


		End		
		Part	:PurpleCrystalScatterPHYS.part
		LifeSpan	230
		LifeSpanJitter	130
	End


End

End
