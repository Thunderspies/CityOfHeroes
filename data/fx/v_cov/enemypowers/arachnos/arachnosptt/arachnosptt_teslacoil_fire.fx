#########################################################
##	forceBubble
########################################################

FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

Input  
	Inpname	HandL
End

LifeSpan	100

########################################################
## Sound FX
########################################################

Condition

	On 	time
	Time 	3
	
	Event
		Type	Local
	
		At	origin

	Sound elecfry 80 80 1
	end
end


Condition

	On 	time
	Time 	15
	
	Event
		Type	Local
	
		At	origin
	Sound ThunderCrack 80 80 .8
	end
end


########################################################
## Visual FX
########################################################

Condition
	On 	Time
	Time 	0




	Event
		EName	BoltOffset
		Type	local
		At	Root
		#bhvr	V_COV\EnemyPowers\Arachnos\mu_Elec_bolt_offset1.bhvr
		BhvrOverride
			PositionOffset		0.00 7.25 0.00
		End
		#Geom    Testing_Target
	End	

End


Condition
	On 	time
	Time 	6
	Event
		EName	asplode
		Type	local
		At	BoltOffset
		bhvr	V_COV\EnemyPowers\Arachnos\mu_Elec_Splosion1.bhvr
		#Geom    Testing_Target
	End
	Event
		Type	local
		At	asplode
		Part1	V_COV\EnemyPowers\Arachnos\mu_Elec_Redball1.part
	End
End


Condition
	On 	time
	Time 	0
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
	Event
		Type	start
		childFX	V_COV\EnemyPowers\Arachnos\mu_Elec_Boltball.fx
	End		
End

Condition
	On 	Time
	Time 	15
	Event
		Type	local
		childFX	V_COV\EnemyPowers\Arachnos\mu_LightningCrack1.fx
		#Geom    Testing_Target
	End		
	
End