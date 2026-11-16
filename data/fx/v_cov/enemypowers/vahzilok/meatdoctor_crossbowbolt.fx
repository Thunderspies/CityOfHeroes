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
#		###Geom	Testing_Target
	End

	Event	
		EName	Barrel
		Type	Local
		At	Emitter
		bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowArrowStart.bhvr
#		Geom	CrossBowBolt
#		##Geom	Testing_Target
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
		At	Emitter
		Bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_CrossBowTravel.bhvr
		Geom	CrossBowBolt
		Sound Crossbow1 100.0 100.0 .7

		Magnet	Target
		LookAt	Target
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


End				