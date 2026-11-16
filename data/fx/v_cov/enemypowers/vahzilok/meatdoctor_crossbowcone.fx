#########################################################
##	
##

FxInfo

LifeSpan  190

#################################################################################
########################   e m i t t e r    p o s   #############################
#################################################################################

Condition
	On 	Time
	Time 	0

	Event	
		EName	Emitter
		Type	Local
		At	LArmL

		bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowEmit.bhvr
##		#Geom	Testing_Target
	End

	Event	
		EName	Barrel
		Type	Local
		At	Emitter
		bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowArrowStart.bhvr
#		Geom	CrossBowBolt
	End
End

#################################################################################
############################   Target  Dummies   ################################
#################################################################################

Condition
	On	Time
	Time	0					## Parent spawned to establish local orientation for targets

	Event
		Ename	TargetParent01			
		Type	Local
		At	Root
		Bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowTarget.bhvr
		 
		
#		#Geom	Testing_Target			## reference object used to see target position, leaving in for debugging help if needed
		Lifespan 100
	End
End
	
Condition
	On	Time
	Time	8					## Parent spawned to establish local orientation for targets

	Event
		Ename	TargetParent			
		Type	Start
		At	TargetParent01
		
		##Geom	Testing_Target			## reference object used to see target position, leaving in for debugging help if needed
		Lifespan 100
	End
End

################### End Target Parent start spawning of dummy Targets ###################

Condition
	On	Time
	Time	8					## Parent spawned to establish local orientation for targets


	Event
		Ename	Target01			## Target node spawned relative to "TargetParent"
		Type	Local
		At	TargetParent
		Bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowTargetR.bhvr
		 
		
#		#Geom	Testing_Target			
		Lifespan 100
	End

	Event
		Ename	Target02			## Target node spawned relative to "TargetParent"
		Type	Local
		At	TargetParent
		Bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowTargetL.bhvr
		 
		
#		#Geom	Testing_Target			
		Lifespan 100
	End

	Event
		Ename	Target03			## Target node spawned relative to "TargetParent"
		Type	Start
		At	TargetParent
		Bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowTargetRandom.bhvr
		 
		
#		#Geom	Testing_Target			
		Lifespan 100
	End
End



#################################################################################
##################################  P R I M E  ##################################
#################################################################################

Condition
	On 	Time
	Time 	29

	Event
		EName 	Prime
		Type	start
		At	Barrel
		Bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowTravel.bhvr
		Geom	CrossBowBolt
		Sound Crossbow1 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime1
		Type	start
		At	Barrel
		Bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowMiss.bhvr
		Geom	CrossBowBolt
		CThresh	1

		Magnet	Target01
#		LookAt	Target01
	End

	Event
		EName 	Prime2
		Type	start
		At	Barrel
		Bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowMiss.bhvr
		Geom	CrossBowBolt

		Magnet	Target02
		LookAt	Target02
	End

	Event
		EName 	Prime3
		Type	start
		At	Barrel
		Bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowMiss.bhvr
		Geom	CrossBowBolt

		Magnet	Target03
		LookAt	Target03
	End

	Event
		Ename	EmitCloud
		Type	Local
		At	Barrel

		Part1	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_CrossBowGas.part
		lifespan 5
	End
End

##################################


Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		Ename 	Barrel
		Type	Destroy
	End

	Event
		Ename 	Emitter
		Type	Destroy
	End
End

Condition
	On 	Prime1Hit

	Event
		Ename 	Prime1
		Type	Destroy
	End

	Event
		Ename	HitCloud01
		Type	Start
		At	Prime1

		Part1	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_CrossBowImpactGas.part
		lifespan 1
	End
End

Condition
	On 	Prime2Hit

	Event
		Ename 	Prime2
		Type	Destroy
	End

	Event
		Ename	HitCloud02
		Type	Start
		At	Prime2

		Part1	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_CrossBowImpactGas.part
		lifespan 1
	End
End

Condition
	On 	Prime3Hit

	Event
		Ename 	Prime3
		Type	Destroy
	End

	Event
		Ename	HitCloud03
		Type	Start
		At	Prime3

		Part1	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_CrossBowImpactGas.part
		lifespan 1
	End
End

End				