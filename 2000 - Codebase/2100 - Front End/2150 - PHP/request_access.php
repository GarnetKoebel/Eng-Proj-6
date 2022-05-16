<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Request Access</title>
</head>
<body>
    <h1>Request Access on server side</h1>
    <?php echo $_POST['firstName'] ?> <br/>
    <?php echo $_POST['lastName'] ?> <br/>
    <?php echo $_POST['email'] ?> <br/>
    <?php echo $_POST['DOB'] ?> <br/>
    <?php echo $_POST['Website'] ?> <br/>
    <?php echo $_POST['fac_or_student'] ?> <br/>
    <?php foreach($_POST['involvement'] as $each) {
        echo $each;
    } ?> <br/>>
    <?php echo $_POST['driver'] ?> <br/>
    <?php echo $_POST['info'] ?> <br/>

</body>
</html>