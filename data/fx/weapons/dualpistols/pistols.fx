#############################################################
## Pistols.fx
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	start
		At	BendMystic
		Part1	WEAPONFX\Gunfire_pistolMuzzflash.part
		Part2	WEAPONFX\Gunfire_pistolsmoke.part
		Part3	WEAPONFX\Gunfire_pistolMuzzflashSmall.part
		#Sound 	techpistol2 100.0 30.0 0.97
		Lifespan 4
	End

	Event
		Type	start
		At	BendMystic
		ChildFX	V_COV\PhysicsEnabled\PistolShellEject.fx
		LifeSpan 70
	End
End

#############################################################

End